#ifndef WINGTOOLPYSERVICE_H
#define WINGTOOLPYSERVICE_H

#include "../WingTool/plugin/iwingtoolplg.h"
#include "PythonQt/PythonQtCppWrapperFactory.h"
#include "PythonQt/PythonQt_QtAll.h"
#include "PythonQt/gui/PythonQtScriptingConsole.h"
#include "scriptcenterwindow.h"
#include "scriptwindow.h"
#include <DDBusSender>
#include <QMutex>
#include <QObject>

class ServiceWrapper;

class WingToolPyService : public QObject {
  Q_OBJECT
public:
  explicit WingToolPyService(IWingToolPlg *plg, QObject *parent = nullptr);
  ~WingToolPyService();

  // 用于本地化使用
  void translation() {
    tr("showEditor");
    tr("showConsole");
    tr("showScriptCenter");
    tr("runScript");
    tr("runScriptFile");
  }

public slots:
  // 显示脚本编辑器
  PLUGINSRV void showEditor();
  // 显示控制台
  PLUGINSRV void showConsole();
  // 显示脚本中心
  PLUGINSRV void showScriptCenter();
  // 执行脚本
  PLUGINSRV bool runScript(QString content);
  // 运行脚本文件
  PLUGINSRV bool runScriptFile(QString filename);
  // DBUS 是 Linux 一个十分重要的特性，拥有它将会有更为强大的功能，简单封装一下
  // 虽然 py 有对应的库，但内置感觉更好
  PLUGININT QList<QVariant> dbusCall(QString service, QString path,
                                     QString interface, QString method,
                                     QList<QVariant> params,
                                     bool isSessionBus = true);

private:
  ScriptWindow *scriptwin;
  ScriptCenterWindow *centerwin;
  PythonQtScriptingConsole *console;
  PythonQtObjectPtr mainContext;

  QMutex mutex;
  ServiceWrapper *wrapper;

  RemoteCallError lasterr = RemoteCallError::Success;
};

class ServiceWrapper : public QObject {
  Q_OBJECT
public:
  ServiceWrapper() {}
  ServiceWrapper(IWingToolPlg *plg, WingToolPyService *srv)
      : m_srv(srv), m_plg(plg) {}
  virtual ~ServiceWrapper() {}

public slots:

  // 为什么这里不用 QVector 而是 QList ，
  // 因为它不支持啊，源代码又懒得看和修改，就这样算了
  void dbusCall(QString service, QString path, QString interface,
                QString method, QList<QVariant> params) {
    m_srv->dbusCall(service, path, interface, method, params);
  }

  QVariant remoteCall(const QString provider, const QString callback,
                      QList<QVariant> params, RemoteCallError &err) {
    return m_plg->remoteCall(provider, callback, params.toVector(), err);
  }
  QVariant sendRemoteMessage(const QString provider, int id,
                             QList<QVariant> params, RemoteCallError &err) {
    return m_plg->sendRemoteMessage(provider, id, params, err);
  }

  bool isProviderExists(const QString provider) {
    return m_plg->isProviderExists(provider);
  }

  bool isServiceExists(const QString provider, const QString callback) {
    return m_plg->isServiceExists(provider, callback);
  }

  bool isInterfaceExists(const QString provider, const QString callback) {
    return m_plg->isInterfaceExists(provider, callback);
  }

  QList<int> getServiceParamTypes(const QString provider,
                                  const QString callback) {
    return m_plg->getServiceParamTypes(provider, callback);
  }

  QStringList getInterfaceParamTypes(const QString provider,
                                     const QString callback) {
    auto lists = m_plg->getInterfaceParamTypes(provider, callback);
    QStringList res;
    for (auto &item : lists) {
      QStringList items;
      for (auto i : item) {
        items.append(QString::number(i));
      }
      res.append(items.join(','));
    }
    return res;
  }

  Qt::KeyboardModifiers getPressedKeyModifiers() {
    return m_plg->getPressedKeyModifiers();
  }

  Qt::MouseButtons getPressedMouseButtons() {
    return m_plg->getPressedMouseButtons();
  }

  QStringList getPluginProviders() { return m_plg->getPluginProviders(); }

  WingPluginInfo getPluginInfo(const QString provider) {
    return m_plg->getPluginInfo(provider);
  }

  QStringList getPluginServices(const QString provider, bool isTr = false) {
    return m_plg->getPluginServices(provider, isTr);
  }

  QStringList getPluginInterfaces(const QString provider) {
    return m_plg->getPluginInterfaces(provider);
  }

private:
  WingToolPyService *m_srv;
  IWingToolPlg *m_plg;
};

class RemoteCallErrorWrapper : public QObject {
  Q_OBJECT
public slots:
  RemoteCallError *new_RemoteCallError() {
    return new RemoteCallError(RemoteCallError::Success);
  }
  void delete_RemoteCallError(RemoteCallError *err) { delete err; }
  int value(RemoteCallError *err) { return int(*err); }
  void setValue(RemoteCallError *err, int value) {
    *err = RemoteCallError(value);
  }

  QString toString(RemoteCallError *err) {
    switch (*err) {
    case RemoteCallError::Success:
      return "Success";
      break;
    case RemoteCallError::Unkown:
      return "Unkown";
      break;
    case RemoteCallError::PluginNotFound:
      return "PluginNotFound";
      break;
    case RemoteCallError::ServiceNotFound:
      return "ServiceNotFound";
      break;
    case RemoteCallError::ArgError:
      return "ArgError";
      break;
    case RemoteCallError::MessageIDError:
      return "MessageIDError";
    }
    return QString();
  }
};

#endif // WINGTOOLPYSERVICE_H
