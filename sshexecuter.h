#ifndef SSHEXECUTER_H
#define SSHEXECUTER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QMap>
#include <QPair>

class SshExecuter : public QObject
{
    Q_OBJECT
public:
    SshExecuter(QObject *parent);
    void toSsh(const QString &host, const QJsonArray bashes, const QString &hostSender);

    QMap<QString, bool> isOnline(const QJsonArray hosts, const QString &hostSender);
private slots:
    void onResultProcess();
    void onFinishedProcess(const int, QProcess::ExitStatus);
signals:
    void finished(QString, QString);
private:
    const QString SSH = "/bin/bash -c \"ssh ";
    const QString FLAGS = " -o StrictHostKeyChecking=no"
                     " -o UserKnownHostsFile=/dev/null";

    QMap <QProcess*, QPair<QString, QString>> _procMapData;
    QString _procData;
    void exec(const QString &sshBash, const QString &hostSender);
};

#endif // SSHEXECUTER_H
