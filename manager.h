#ifndef MANAGER_H
#define MANAGER_H

#include "parser.h"

#include <QObject>
#include <QVector>

class Manager : public QObject
{
    Q_OBJECT
private:
    bool isExecuter(const QString &type);
    QStringList getHostsList(const QString &hallId);
public:
    Manager();
    void taskSwitch(const QString &type, const QJsonObject dataJsonObj);
};

#endif // MANAGER_H
