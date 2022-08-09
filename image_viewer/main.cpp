#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QFileSystemModel>
#include <QUrl>

class FolderSystemModel : public QFileSystemModel{
    Q_OBJECT
public:
    explicit FolderSystemModel(QObject *parent = nullptr) : QFileSystemModel(parent)
    {
        setRootPath(QDir::homePath());
        setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.isValid() && role == Qt::DisplayRole) {
                  return QVariant(fileInfo(index).absoluteFilePath());
        }
        return QFileSystemModel::data(index, role);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QFileSystemModel *fsm = new FolderSystemModel(&engine);
    engine.rootContext()->setContextProperty("fileSystemModel", fsm);
    engine.rootContext()->setContextProperty("rootPathIndex", fsm->index(fsm->rootPath()));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "main.moc"
