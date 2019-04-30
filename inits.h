#ifndef INITS_H
#define INITS_H

#include "serverrm.h"
#include "configs.h"
#include "parser.h"
#include "database.h"
#include "manager.h"

#include <QPointer>

class Inits
{
public:
    Inits(){}
    static Inits& Instance();

    bool isInited();

    DataBase *getDataBase();
    Parser *getParser();
    Configs *getConfigs();
    Manager *getManager();
private:
    Inits(const Inits& root) = delete;
    Inits& operator = (const Inits&) = delete;

    QPointer<ServerRM> _serverRM = nullptr;
    QPointer<Parser> _parser = nullptr;
    QPointer<DataBase> _database = nullptr;
    QPointer<Configs> _configs = nullptr;
    QPointer<Manager> _manager = nullptr;
    bool isInitConfigs();
    bool isInitDB();
    bool isInitParser();
    bool isInitManager();
    bool isInitServer();
};

#endif // INITS_H
