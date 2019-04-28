#ifndef INITS_H
#define INITS_H

#include "serverrm.h"
#include "configs.h"

#include <QPointer>

class Inits
{
public:
    Inits();
    bool isInited();

private:
    QPointer<ServerRM> _serverRM = nullptr;
};

#endif // INITS_H
