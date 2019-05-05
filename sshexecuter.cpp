#include "sshexecuter.h"

#include <QProcess>
#include <QPointer>
#include <QJsonArray>
#include <QDebug>

SshExecuter::SshExecuter()
{

}

void SshExecuter::toSsh(const QString &host, const QJsonArray bashes)
{
    if (!_procData.isEmpty()) {
        _procData.clear();
    }

    foreach(const QJsonValue bash, bashes) {
        const QString &sshBash = SSH + host + FLAGS + " '" + bash.toString() + "'\"";
        qDebug() << sshBash;

        exec(sshBash);
    }
}

QString SshExecuter::exec(const QString &sshBash)
{
    QProcess proc;
    connect(&proc, &QProcess::readyReadStandardOutput, this, &SshExecuter::onResultProcess);

    qDebug() << "Exec bash:" << sshBash;
    proc.start(sshBash);
    proc.waitForFinished();

    if (_procData.isEmpty()) {
        _procData = "Done";
    }

    return _procData;
}

void SshExecuter::onResultProcess()
{
    QPointer<QProcess> proc = qobject_cast<QProcess*>(sender());

    if (!proc) {
        return;
    }

    const QString data = proc->readAllStandardOutput();

    _procData += data;
}
