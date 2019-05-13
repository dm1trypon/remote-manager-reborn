#include "log.h"

#include <QDateTime>

Log::Log(QObject *parent) : QObject(parent)
{

}

QString Log::getDataTime()
{
    return QDateTime::currentDateTime().toTimeSpec(Qt::OffsetFromUTC).toString("[dd.mm.yy hh:mm:ss]");
}
