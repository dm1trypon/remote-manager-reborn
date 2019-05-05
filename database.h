#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QTimer>

class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(const QJsonObject configDB);

    bool isConnected();
    QJsonArray getHostsList(const QString &hallId);
private slots:
    void checkConnection();
private:
    QTimer _tCheckDbConnection;
    QSqlDatabase _db;
    void setDataBase(const QJsonObject configDB);
    QString queryGetIp(const QString &hallId);
    static QString getDataTime();
};

#endif // DATABASE_H
