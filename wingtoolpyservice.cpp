#include "wingtoolpyservice.h"
#include <DWidgetUtil>
#include <QException>
#include <QShortcut>

WingToolPyService::WingToolPyService(IWingToolPlg *plg, QObject *parent)
    : QObject(parent) {
  PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
  PythonQt_QtAll::init();
  auto py = PythonQt::self();
  wrapper = new ServiceWrapper(plg, this);
  py->registerCPPClass("plgservice", nullptr, "wingplg",
                       PythonQtCreateObject<ServiceWrapper>);
  qRegisterMetaType<RemoteCallError>("RemoteCallError");
  py->addDecorators(new RemoteCallErrorWrapper);
  mainContext = PythonQt::self()->getMainModule();
  mainContext.addObject("service", wrapper);
  mainContext.addVariable("lasterr", QVariant::fromValue(lasterr));
  mainContext.evalScript("from PythonQt import *");
  console = new PythonQtScriptingConsole(nullptr, mainContext);
  console->setMinimumSize(300, 200);
  Dtk::Widget::moveToCenter(console);

  scriptwin = new ScriptWindow;
  connect(scriptwin, &ScriptWindow::runPyFile, this,
          &WingToolPyService::runScriptFile);
  connect(scriptwin, &ScriptWindow::runPyScript, this,
          &WingToolPyService::runScript);

  centerwin = new ScriptCenterWindow;
  connect(centerwin, &ScriptCenterWindow::sigRunScript, this,
          &WingToolPyService::runScriptFile);
  Dtk::Widget::moveToCenter(centerwin);

  connect(ScriptManager::instance(), &ScriptManager::runPyFile, this,
          &WingToolPyService::runScriptFile);

  auto sc =
      new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_L), console);
  connect(sc, &QShortcut::activated, console, &PythonQtScriptingConsole::clear);
}

WingToolPyService::~WingToolPyService() {
  console->deleteLater();
  scriptwin->deleteLater();
  centerwin->deleteLater();
}

void WingToolPyService::showEditor() {
  scriptwin->show();
  scriptwin->activateWindow();
  scriptwin->raise();
}

void WingToolPyService::showConsole() {
  console->show();
  console->raise();
}

void WingToolPyService::showScriptCenter() {
  centerwin->show();
  centerwin->raise();
}

bool WingToolPyService::runScript(QString content) {
  if (mutex.tryLock()) {
    if (scriptwin->isVisible())
      showConsole();
    auto cur = console->textCursor();
    cur.movePosition(QTextCursor::End);
    console->setTextCursor(cur);
    console->insertPlainText(tr("[ExcuteFromScriptWindow]"));
    mainContext.evalScript(content);
    console->appendCommandPrompt();
    mutex.unlock();
    return true;
  }
  return false;
}

bool WingToolPyService::runScriptFile(QString filename) {
  if (mutex.tryLock()) {
    if (scriptwin->isVisible())
      showConsole();
    auto cur = console->textCursor();
    cur.movePosition(QTextCursor::End);
    console->setTextCursor(cur);
    console->insertPlainText(filename);
    mainContext.evalFile(filename);
    console->appendCommandPrompt();
    mutex.unlock();
    return true;
  }
  return false;
}

QList<QVariant> WingToolPyService::dbusCall(QString service, QString path,
                                            QString interface, QString method,
                                            QList<QVariant> params,
                                            bool isSessionBus) {
  // 为什么不使用更简单的 DBUS 调用呢？如果有非法调用有异常啊！
  // 这不是 win ，C++ 标准只规定了捕获 throw 出来的异常
  // gcc try-catch 也就只捕获这个
  auto dbus = QDBusMessage::createMethodCall(service, path, interface, method);
  dbus.setArguments(params);
  QDBusMessage response = isSessionBus
                              ? QDBusConnection::sessionBus().call(dbus)
                              : QDBusConnection::systemBus().call(dbus);
  if (response.type() == QDBusMessage::ErrorMessage) {
    console->consoleMessage(
        QString("<font color=\"red\">%1</font>").arg(response.errorMessage()));

  } else if (Q_LIKELY(response.type() == QDBusMessage::ReplyMessage)) {
    return response.arguments();
  }
  return QList<QVariant>();
}
