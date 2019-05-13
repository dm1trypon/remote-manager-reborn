#ifndef SSHEXECUTER_H
#define SSHEXECUTER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QMap>

class SshExecuter : public QObject
{
    Q_OBJECT
public:
    SshExecuter(QObject *parent);
    void toSsh(const QString &host, const QJsonArray bashes);

    QMap<QString, bool> isOnline(const QJsonArray hosts);
private slots:
    void onResultProcess();
    void onFinishedProcess(const int, QProcess::ExitStatus);
signals:
    void finished(QString);
private:
    const QString SSH = "/bin/bash -c \"ssh ";
    const QString FLAGS = " -o StrictHostKeyChecking=no"
                     " -o UserKnownHostsFile=/dev/null";

    QMap <QProcess*, QString> _procMapData;
    QString _procData;
    void exec(const QString &sshBash);
};

#endif // SSHEXECUTER_H
