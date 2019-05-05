#ifndef SSH_H
#define SSH_H

#include <QObject>

class SshExecuter : public QObject
{
    Q_OBJECT
public:
    SshExecuter();
    void toSsh(const QString &host, const QJsonArray bashes);

private slots:
    void onResultProcess();
private:
    const QString SSH = "/bin/bash -c \"ssh ";
    const QString FLAGS = " -o StrictHostKeyChecking=no"
                     " -o UserKnownHostsFile=/dev/null";

    QString _procData;
    QString exec(const QString &sshBash);
};

#endif // SSH_H
