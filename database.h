#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(const QJsonObject configDB);

    bool isConnected();
    QStringList getHostsList(const QString &hallId);
private:
    QSqlDatabase _db;
    void setDataBase(const QJsonObject configDB);
    QString queryGetIp(const QString &hallId);
};

#endif // DATABASE_H
