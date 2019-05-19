#ifndef DEPENDENCE_H
#define DEPENDENCE_H

#include <QObject>
#include <QJsonArray>

class Dependence : public QObject
{
    Q_OBJECT
public:
    Dependence(const QJsonArray depJsArr, QObject *parent);

    bool isExists();
private:
    QString _procData;
    QJsonArray _depJsArr;

private slots:
    void onResultProcess();
};

#endif // DEPENDENCE_H
