#include "sshexecuter.h"
#include "log.h"

#include <QProcess>
#include <QPointer>
#include <QJsonArray>
#include <QDebug>

SshExecuter::SshExecuter(QObject *parent) : QObject (parent)
{

}

void SshExecuter::toSsh(const QString &hostEx, const QString &method, const QJsonArray bashes,
                        const QString &hostSender)
{
    if (!_procData.isEmpty()) {
        _procData.clear();
    }

    foreach(const QJsonValue bash, bashes) {
        const QString &sshBash = SSH + hostEx + FLAGS + " '" + bash.toString() + "'\"";

        infoSshEx << "SSH bash:" << sshBash;

        exec(sshBash, hostSender, method, hostEx);
    }
}

void SshExecuter::exec(const QString &sshBash, const QString &hostSender, const QString &method, const QString &hostEx)
{
    QPointer<QProcess> proc = new QProcess();

    _procResultData.insert(proc, QPair<QPair<QString, QString>, QPair<QString, QString>>
                           (QPair<QString, QString>(hostSender, ""),
                            QPair<QString, QString>(hostEx, method)));


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

    if (!_procResultData.contains(proc)) {
        return;
    }

    QString procData = _procResultData[proc].first.second;

    if (procData.isEmpty()) {
        procData = "Done";
    }

    QStringList finishedData = {
        procData,
        _procResultData[proc].first.first,
        _procResultData[proc].second.first,
        _procResultData[proc].second.second
    };

    _procResultData.remove(proc);

    emit finished(finishedData);
}

QMap<QString, bool> SshExecuter::isOnline(const QJsonArray hosts, const QString &hostSender)
{
    QString bash = "fping";

    foreach(const QJsonValue host, hosts) {
        bash = bash + " " + host.toString();
    }

    QPointer<QProcess> proc = new QProcess;

    _procResultData.insert(proc, QPair<QPair<QString, QString>, QPair<QString, QString>>
                           (QPair<QString, QString>(hostSender, ""),
                            QPair<QString, QString>("", "")));

    connect(proc, &QProcess::readyReadStandardOutput, this, &SshExecuter::onResultProcess);

    proc->start(bash);
    proc->waitForFinished();

    if (!_procResultData.contains(proc)) {
        return {};
    }

    QStringList resultProc = _procResultData[proc].first.second.split("\n");
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

    _procResultData[proc].first.second = _procResultData[proc].first.second + data;
}
