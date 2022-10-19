#ifndef ABOUTSOFTWAREDIALOG_H
#define ABOUTSOFTWAREDIALOG_H

#include <DDialog>
#include <DMainWindow>
#include <QObject>

DWIDGET_USE_NAMESPACE
class AboutDialog : public DDialog {

  Q_OBJECT
public:
  explicit AboutDialog(DMainWindow *parent = nullptr);

signals:

public slots:
};

#endif // ABOUTSOFTWAREDIALOG_H
