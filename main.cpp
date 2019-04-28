#include "inits.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Inits inits;

    if (!inits.isInited()) {
        qWarning() << "Service start failed!";

        return -1;
    }

    return a.exec();
}
