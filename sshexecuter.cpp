#include "sshexecuter.h"
#include "log.h"

#include <QProcess>
#include <QPointer>
#include <QJsonArray>
#include <QDebug>

SshExecuter::SshExecuter(QObject *parent) : QObject (parent)
{

}

void SshExecuter::toSsh(const QString &host, const QJsonArray bashes, const QString &hostSender)
{
    if (!_procData.isEmpty()) {
        _procData.clear();
    }

    foreach(const QJsonValue bash, bashes) {
        const QString &sshBash = SSH + host + FLAGS + " '" + bash.toString() + "'\"";

        infoSshEx << "SSH bash:" << sshBash;

        exec(sshBash, hostSender);
    }
}

void SshExecuter::exec(const QString &sshBash, const QString &hostSender)
{
    QPointer<QProcess> proc = new QProcess();

    _procMapData.insert(proc, QPair<QString, QString>(hostSender, ""));

    connect(proc, &QProcess::readyReadStandardOutput, this, &SshExecuter::onResultProcess);
    connect(proc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this,
                     &SshExecuter::onFinishedProcess);

    proc->start(sshBash);
}

void SshExecuter::onFinishedProcess(const int, QProcess::ExitStatus)
{
    QPointer<QProcess> proc = qobject_cast<QProcess*>(sender());

    if (!proc) {
        return;
    }

    if (!_procMapData.contains(proc)) {
        return;
    }

    QString procData = _procMapData[proc].second;

    const QString hostSender = _procMapData[proc].first;

    _procMapData.remove(proc);

    if (procData.isEmpty()) {
        procData = "Done";
    }

    emit finished(procData, hostSender);
}

QMap<QString, bool> SshExecuter::isOnline(const QJsonArray hosts, const QString &hostSender)
{
    QString bash = "fping";

    foreach(const QJsonValue host, hosts) {
        bash = bash + " " + host.toString();
    }

    QPointer<QProcess> proc = new QProcess;

    _procMapData.insert(proc, QPair<QString, QString>(hostSender, ""));

    connect(proc, &QProcess::readyReadStandardOutput, this, &SshExecuter::onResultProcess);

    proc->start(bash);
    proc->waitForFinished();

    if (!_procMapData.contains(proc)) {
        return {};
    }

    QStringList resultProc = _procMapData[proc].second.split("\n");
    resultProc.removeOne("");

    QMap<QString, bool> hostsStatus;

    foreach(const QString host, resultProc) {
        if (host.contains("is alive")) {
            hostsStatus.insert(host.split(" ").first(), true);

            continue;
        }

        hostsStatus.insert(host.split(" ").first(), false);
    }

    return hostsStatus;
}

void SshExecuter::onResultProcess()
{
    QPointer<QProcess> proc = qobject_cast<QProcess*>(sender());

    if (!proc) {
        return;
    }

    const QString data = proc->readAllStandardOutput();

    _procMapData[proc].second = _procMapData[proc].second + data;
}
