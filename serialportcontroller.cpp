#include "serialportcontroller.h"

SerialportController::SerialportController(QObject *parent)
    : QSerialPort(parent)
{
    qDebug() << this << "Serialport Controller Class constructed!";

}

SerialportController::~SerialportController()
{
    qDebug() << this << "Serialport Controller Class deconstructed..";

}

void SerialportController::transaction(const QString &portName, int waitTimeOut, const QString &request)
{
    myportName = portName;



}

QList<QString> SerialportController::ListSerialPorts()
{
    QList<QString> portInfoText;

    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &iterator : infos)
    {
        QString infoString =  QObject::tr("Port: ") + iterator.portName() + "\n"
                           +  QObject::tr("Location: ") + iterator.systemLocation() + "\n"
                           +  QObject::tr("Description: ")  + iterator.description()  + "\n"
                           +  QObject::tr("Manufacturer: ") + iterator.manufacturer()  + "\n"
                           +  QObject::tr("Serial Number: ") + iterator.serialNumber()+ "\n"
                           +  QObject::tr("Vendor Identifier: ") + (iterator.hasVendorIdentifier() ? QString::number(iterator.vendorIdentifier(), 16) : QString())  + "\n"
                           +  QObject::tr("Product Identifier: ") + (iterator.hasProductIdentifier() ? QString::number(iterator.productIdentifier(), 16) : QString())  + "\n";

        portInfoText.append(infoString);
        emit portNameSignal(iterator.portName());
    }

    return portInfoText;
}


