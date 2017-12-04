#ifndef TRANSFERENTRY_H
#define TRANSFERENTRY_H

#include <QObject>
#include <QNetworkReply>

#include <qwebdav.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <sys/stat.h>

#include "nextcloudsettings.h"
#include "shellcommand.h"
#include "webdav_utils.h"

class TransferEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString localPath READ getLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(qint64 size READ getSize NOTIFY sizeChanged)
    Q_PROPERTY(int direction READ getTransferDirection)

public:
    enum TransferDirection {
        DOWN, UP
    };

    Q_ENUMS(TransferDirection)

    TransferEntry(QObject *parent = 0, QWebdav *webdav = 0,
                  QString name = "", QString remotePath = "",
                  QString localPath = "", qint64 size = 0,
                  TransferDirection direction = DOWN,
                  bool open = false,
                  QStringList pathsToCreate = QStringList());

    ~TransferEntry();

    QStringList getCreatedPaths();
    bool hasPathsToCreate();

    Q_INVOKABLE QString getName();
    QString getLocalPath();
    // XXX: Q_INVOKABLE getRemotePath can be called in QML if navigation gets fixed, see FileBrowser.qml
    Q_INVOKABLE QString getRemotePath();
    qint64 getSize();
    QDateTime getLastModified();
    qreal getProgress();
    int getTransferDirection();

    void setLastModified(QDateTime lastModified);
    void setProgress(qreal value);

    void startTransfer();
    Q_INVOKABLE void cancelTransfer();
    bool succeeded();

private:
    void createDirectory();
    void setRemoteLastModified();
    void setLocalLastModified();

    QWebdav *webdav;
    QNetworkReply *networkReply;
    QFile *localFile;
    QFileInfo *localFileInfo;

    bool m_open;

    QStringList m_pathsToCreate;
    QStringList m_createdPaths;

    QString m_name;
    QString m_localPath;
    QString m_remotePath;
    qint64 m_size;
    QDateTime m_lastModified;
    qreal m_progress;
    TransferDirection m_direction;

signals:
    void nameChanged();
    void localPathChanged();
    void sizeChanged();
    void progressChanged(qreal progress, QString remotePath);
    void transferCompleted(bool success);
    void localMtimeFailed(int status);
    void remoteMtimeFailed(int status);
    void remoteMtimeSucceeded();

public slots:
    void handleReadComplete();
    void handleProgressChange(qint64 bytes, qint64 bytesTotal);
    void setRemoteMtimeFinished(QNetworkReply* networkReply);

private slots:
    void errorHandler(QNetworkReply::NetworkError);
    void resetReply();

};

#endif // TRANSFERENTRY_H
