#include "wingtoolpy.h"
#include "aboutdialog.h"
#include "scriptmanager.h"
#include <QMessageBox>

WingToolPy::WingToolPy(QObject *parent) { Q_UNUSED(parent); }

int WingToolPy::sdkVersion() { return SDKVERSION; }

QString WingToolPy::signature() { return WINGSUMMER; }

WingToolPy::~WingToolPy() { m_menu->deleteLater(); }

bool WingToolPy::preInit() {
  service = new WingToolPyService(this);
  return true;
}

bool WingToolPy::init(QList<WingPluginInfo> loadedplugin) {
  Q_UNUSED(loadedplugin);
  m_menu = new QMenu(tr("ScriptCenter"));
  m_menu->setIcon(QIcon(":/WingToolPy/img/icon.png"));
  ScriptManager::instance()->loadMenu(m_menu);
  return true;
}

void WingToolPy::unload() { service->deleteLater(); }

QString WingToolPy::pluginName() { return tr("WingToolPy"); }

QString WingToolPy::pluginAuthor() { return WINGSUMMER; }

IWingToolPlg::Catagorys WingToolPy::pluginCatagory() {
  return IWingToolPlg::Catagorys::Develop;
}

uint WingToolPy::pluginVersion() { return 1; }

QString WingToolPy::pluginComment() {
  return tr("A powerful plugin to support PyScript automation!");
}

QString WingToolPy::pluginWebsite() {
  return "https://code.gitlink.org.cn/wingsummer/WingToolPy";
}

QIcon WingToolPy::pluginIcon() { return QIcon(":/WingToolPy/img/icon.png"); }

const QMetaObject *WingToolPy::serviceMeta() { return service->metaObject(); }

const QPointer<QObject> WingToolPy::serviceHandler() { return service; }

QObject *WingToolPy::trayRegisteredMenu() { return m_menu; }

QString WingToolPy::translatorFile() { return "WingToolPy.qm"; }

QVariant WingToolPy::pluginServicePipe(int serviceID, QList<QVariant> params) {
  Q_UNUSED(serviceID);
  Q_UNUSED(params);
  return QVariant();
}

void WingToolPy::onPluginCenter() {
  AboutDialog d;
  d.exec();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(WingToolPy, GenericPlugin)
#endif // QT_VERSION < 0x050000
