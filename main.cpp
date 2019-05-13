#include "inits.h"
#include <QJsonArray>

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "[[[ STARTING REMOTE MANAGER SERVICE ]]]";

    if (!Inits::Instance().isInited()) {
        qCritical() << "[[[ SERVICE START FAILED ]]]";

        return -1;
    }

    qDebug() << "[[[ SERVICE HAS BEEN STARTED ]]]";

    return a.exec();
}
