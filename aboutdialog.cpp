#include "aboutdialog.h"
#include <DLabel>
#include <DTextBrowser>
#include <QPixmap>

AboutDialog::AboutDialog(DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("About"));

  auto l = new DLabel(this);
  l->setFixedSize(100, 100);
  l->setScaledContents(true);
  l->setPixmap(QPixmap(":/images/author.jpg"));
  addContent(l, Qt::AlignHCenter);
  addSpacing(10);
  auto b = new DTextBrowser(this);

  b->setSearchPaths(QStringList({":/WingToolPy/img"}));

  b->setSource(QUrl("README.md"), QTextDocument::MarkdownResource);

  b->setFixedSize(800, 500);
  b->setOpenExternalLinks(true);
  addContent(b);
}
