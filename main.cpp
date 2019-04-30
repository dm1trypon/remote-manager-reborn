#include "inits.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "[[[ STARTING REMOTE MANAGER SERVICE ]]]";

    if (!Inits::Instance().isInited()) {
        qWarning() << "[[[ SERVICE START FAILED ]]]";

        return -1;
    }

    qDebug() << "[[[ SERVICE HAS BEEN STARTED ]]]";

    return a.exec();
}
