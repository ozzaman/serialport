#ifndef SERIALPORTCONTROLLER_H
#define SERIALPORTCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QDebug>

class SerialportController : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialportController(QObject *parent = nullptr);
    ~SerialportController();

    void transaction(const QString &portName, int waitTimeOut, const QString &request);

signals:
    void portNameSignal(QString);

public:
    QList<QString> ListSerialPorts();

private:
    QString myportName;
    int mytimeOut;
    QString myrequest;

};

#endif // SERIALPORTCONTROLLER_H
