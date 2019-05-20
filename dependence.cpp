#include "dependence.h"
#include "log.h"

#include <QProcess>
#include <QPointer>
#include <QJsonArray>
#include <QDebug>

Dependence::Dependence(const QJsonArray depJsArr, QObject *parent) :
    QObject(parent),
    _depJsArr(depJsArr)
{

}

bool Dependence::isExists()
{
    QPointer<QProcess> proc = new QProcess();

    connect(proc, &QProcess::readyReadStandardOutput, this, &Dependence::onResultProcess);

    foreach(const QJsonValue dependence, _depJsArr) {
        if (!_procData.isEmpty()) {
            _procData.clear();
        }

        proc->start("which " + dependence.toString());
        proc->waitForFinished();

        if (isIncorrect()) {
            warnDependence << dependence.toString() << "is not found";

            return false;
        }
    }

    return true;
}

bool Dependence::isIncorrect()
{
    return _procData.isEmpty() || _procData.contains("which: no");
}

void Dependence::onResultProcess()
{
    QPointer<QProcess> proc = qobject_cast<QProcess*>(sender());

    if (!proc) {
        return;
    }

    _procData = proc->readAllStandardOutput();
}
