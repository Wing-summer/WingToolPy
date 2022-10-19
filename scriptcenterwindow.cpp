#include "scriptcenterwindow.h"
#include <DPushButton>
#include <DTextBrowser>
#include <DTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

ScriptCenterWindow::ScriptCenterWindow(DMainWindow *parent) : DDialog(parent) {
  setMinimumSize(700, 500);
  setWindowTitle(tr("ScriptCenter"));
  auto w = new QWidget(this);
  auto layout = new QHBoxLayout(w);
  auto tree = new QTreeWidget(this);
  tree->setHeaderHidden(true);
  m = ScriptManager::instance();
  m->loadTreeWidget(tree);
  layout->addWidget(tree);

  auto vlayout = new QVBoxLayout;
  layout->addLayout(vlayout);
  auto txt = new QTextBrowser(this);
  vlayout->addWidget(txt);
  addContent(w);

  auto runbtn = new DPushButton(tr("Run"), this);
  runbtn->setEnabled(false);
  connect(tree, &QTreeWidget::itemSelectionChanged, this, [=] {
    auto s = tree->selectedItems();
    if (s.count()) {
      auto si = s.first();
      auto meta = si->data(0, Qt::UserRole).value<ScriptMeta>();
      if (meta.name.length()) {
        txt->setMarkdown(QString("**%1** : %2\n\n**%3** : %4\n\n"
                                 "**%5** : %6\n\n**%7** : %8\n\n"
                                 "**%9** : %10\n\n")
                             .arg(tr("Name"))
                             .arg(meta.name)
                             .arg(tr("Author"))
                             .arg(meta.author)
                             .arg(tr("License"))
                             .arg(meta.license)
                             .arg("Version")
                             .arg(meta.version)
                             .arg("Commnet")
                             .arg(meta.commnet));
        runbtn->setEnabled(true);
      } else {
        txt->setMarkdown(
            QString("**%1** : %2\n\n").arg(tr("Catagory")).arg(si->text(0)));
        runbtn->setEnabled(false);
      }
    }
  });

  connect(runbtn, &DPushButton::clicked, this, [=] {
    auto s = tree->selectedItems();
    if (s.count()) {
      auto si = s.first();
      auto meta = si->data(0, Qt::UserRole).value<ScriptMeta>();
      if (meta.name.length()) {
        emit this->sigRunScript(meta.filename);
      }
    }
  });
  vlayout->addWidget(runbtn);
}
