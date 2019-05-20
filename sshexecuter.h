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
    void toSsh(const QString &hostEx, const QString &method, const QJsonArray bashes, const QString &hostSender);

    QMap<QString, bool> isOnline(const QJsonArray hosts, const QString &hostSender);
private slots:
    void onResultProcess();
signals:
    void finished(QMap<QString, QString>);
private:
    const QString SSH = "/bin/bash -c \"ssh ";
    const QString FLAGS = " -o StrictHostKeyChecking=no"
                     " -o UserKnownHostsFile=/dev/null";

    QMap <QProcess*, QPair<QPair<QString, QString>, QPair<QString, QString>>> _procResultData;

    QString _procData;
    void exec(const QString &sshBash, const QString &hostSender, const QString &method, const QString &hostEx, QProcess *proc);
};

#endif // SSHEXECUTER_H
