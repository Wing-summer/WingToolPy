#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "../WingTool/plugin/iwingtoolplg.h"
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QTreeWidget>

struct ScriptMeta {
  QString name;
  QString filename;
  QString author;
  QString license;
  QString commnet;
  uint version;
};

Q_DECLARE_METATYPE(ScriptMeta)

class ScriptManager : public QObject {
  Q_OBJECT
public:
  static ScriptManager *instance();
  void loadMenu(QMenu *menu);
  void loadTreeWidget(QTreeWidget *tree);
  void parseMeta(QFileInfo fileinfo, QString folder, QList<ScriptMeta> &m);
  static ScriptManager *m_instance;

signals:
  bool runPyFile(QString filename);

private:
  ScriptManager(QObject *parent = nullptr);
  QMap<QString, QList<ScriptMeta>> m_metas;
};

#endif // SCRIPTMANAGER_H
