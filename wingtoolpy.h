#ifndef GENERICPLUGIN_H
#define GENERICPLUGIN_H

#include "../WingTool/plugin/iwingtoolplg.h"
#include "wingtoolpyservice.h"
#include <QTranslator>

class WingToolPy : public IWingToolPlg {
  Q_OBJECT
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IWINGPLUGIN_INTERFACE_IID FILE "WingToolPy.json")
#endif // QT_VERSION >= 0x050000

  Q_INTERFACES(IWingToolPlg)

public:
  WingToolPy(QObject *parent = nullptr);
  int sdkVersion() override;
  QString signature() override;
  ~WingToolPy() override;

  bool preInit() override;
  bool init(QList<WingPluginInfo> loadedplugin) override;
  void unload() override;
  QString pluginName() override;
  QString pluginAuthor() override;
  Catagorys pluginCatagory() override;
  uint pluginVersion() override;
  QString pluginComment() override;
  QString pluginWebsite() override;
  QIcon pluginIcon() override;
  const QMetaObject *serviceMeta() override;
  const QPointer<QObject> serviceHandler() override;
  QObject *trayRegisteredMenu() override;
  QString translatorFile() override;

public slots:
  QVariant pluginServicePipe(int serviceID, QList<QVariant> params) override;
  virtual void onPluginCenter() override;

private:
  WingToolPyService *service;
  QMenu *m_menu;
};

#endif // GENERICPLUGIN_H
