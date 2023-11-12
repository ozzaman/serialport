#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    QObject::connect(ui->actionList_Available_Ports, &QAction::triggered, this, &MainWindow::listAvailablePorts);
    QObject::connect(ui->actionDisconnet_from_Port, &QAction::triggered, this, &MainWindow::closeSerialPort);
    QObject::connect(goSerialPort, &SerialportController::portNameSignal, this, &MainWindow::populatePortsCombo);
//    QObject::connect(ui->portsCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::populateBaudRateCombo);
    QObject::connect(ui->portsCombo, qOverload<int>(&QComboBox::activated), this, &MainWindow::onportsComboActivated);
    QObject::connect(ui->serialPortCntBtn, &QPushButton::clicked, this, &MainWindow::onserialPortCntBtnClicked);
    QObject::connect(ui->writeToPortBtn, &QPushButton::clicked, this, &MainWindow::onwriteToPortBtnClicked);
    QObject::connect(goSerialPort, &SerialportController::readyRead, this, &MainWindow::readReceviedPortData);
    QObject::connect(ui->actionDisconnet_from_Port, &QAction::triggered, goSerialPort, &SerialportController::close);
    QObject::connect(ui->baudRatesCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &::MainWindow::onbaudRatesComboCurrentIndexChanged);
    QTimer *connectionCheckTimer = new QTimer(this);
    QObject::connect(connectionCheckTimer, &QTimer::timeout, this, &MainWindow::listAvailablePorts);
    connectionCheckTimer->start(500);
    goSerialPort->setBaudRate(QSerialPort::Baud9600);  //Default BaudRate if no change
    this->populateBaudRateCombo(1);
    QObject::connect(ui->readFileBtn, &QPushButton::clicked, this, &MainWindow::onreadFileBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::listAvailablePorts()
{    
    ui->listWidget->clear();
    ui->portsCombo->clear();
    portsList = goSerialPort->ListSerialPorts();
    ui->listWidget->addItems(portsList);

}

void MainWindow::populatePortsCombo(QString portName)
{
    ui->portsCombo->addItem(portName);    

}

void MainWindow::populateBaudRateCombo(int portID)
{
    if(portID == -1)
        return;

    ui->baudRatesCombo->clear();

//    goSerialPort->setPortName(ui->portsCombo->currentText());
    availableBaudRatesList = QSerialPortInfo::standardBaudRates();
//    qDebug() << baudRatesAvailable.size();
/*    for(int i = 20; i < availableBaudRatesList.size(); i++){
        ui->baudRatesCombo->addItem(QString::number(baudRatesAvailable.at(i)));
    }*/
    for (qint32 baudRateIterator : availableBaudRatesList){
        if(baudRateIterator > 4800)
            ui->baudRatesCombo->addItem(QString::number(baudRateIterator));
    }

    ui->baudRatesCombo->addItem(tr("Custom")); //Implement QIntValidator !
}

void MainWindow::onportsComboActivated(int index)
{
    Q_UNUSED(index)

    goSerialPort->setPortName(ui->portsCombo->currentText());
    /*
    goSerialPort->setBaudRate(QSerialPort::Baud9600);
    goSerialPort->open(QIODevice::ReadWrite);

    if(goSerialPort->isOpen()){
        statusBar()->setStyleSheet("color: green");
        statusBar()->showMessage(goSerialPort->portName() + " opened successfully at default baud rate:" + QString::number(goSerialPort->baudRate()), 3000);
    }
    else{
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage(goSerialPort->portName() + " could not be opened.. " + goSerialPort->errorString() , 3000);
    }
    */
}

void MainWindow::readReceviedPortData()
{
    const QByteArray receivedData = goSerialPort->readAll();
    ui->receivedPlTxtEdt->insertPlainText(receivedData);

}

void MainWindow::onwriteToPortBtnClicked()
{
    currentRequest = ui->requestLnEdt->text();
    const QByteArray requestData = currentRequest.toUtf8();
//    const QByteArray requestData("\xFF");
    const qint64 bytesWritten = goSerialPort->write(requestData);

    if(bytesWritten == -1){
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage(tr("Failed to write to port %1, the error is: %2").arg(goSerialPort->portName()).arg(goSerialPort->errorString()));
    }
    else if(bytesWritten != requestData.size()){
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage(tr("Not all the data could be written to %1, the error is: %2").arg(goSerialPort->portName()).arg(goSerialPort->errorString()));
    }
    else{
        statusBar()->setStyleSheet("color: green");
        statusBar()->showMessage(tr("%1 bytes written to %2").arg(bytesWritten).arg(goSerialPort->portName()));
    }
}

void MainWindow::onbaudRatesComboCurrentIndexChanged(int index)
{
    qint32 setBaudRate = ui->baudRatesCombo->currentText().toInt();
    goSerialPort->setBaudRate(setBaudRate);
}

void MainWindow::closeSerialPort()
{
    if(goSerialPort->isOpen()){
        goSerialPort->close();
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage(goSerialPort->portName() + " is closed..");
    }
    else{
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage("No opened port to close !");
    }
}


void MainWindow::onserialPortCntBtnClicked()
{
    if(goSerialPort->openMode() == QIODeviceBase::NotOpen){
        goSerialPort->setPortName(ui->portsCombo->currentText());
        statusBar()->showMessage(goSerialPort->portName() + " opened succesfully at baudrate set to: " + QString::number(goSerialPort->baudRate()), 3000);
        goSerialPort->open(QIODevice::ReadWrite);
        ui->serialPortCntBtn->setText("Disconnect");
    }
    else{
        goSerialPort->close();
        statusBar()->setStyleSheet("color: red");
        statusBar()->showMessage(goSerialPort->portName() + " is closed..");
        ui->serialPortCntBtn->setText("Connect");
    }
}

void MainWindow::fileOpen()
{





}




void MainWindow::onreadFileBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/go/Qt_Projects/Serialport_Protocol/", tr("*.csv"), nullptr, QFileDialog::DontUseNativeDialog); //Change the Path !

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream textToBeRead(&file);

    QString line = textToBeRead.readLine(); // Model file first line is the header. We read and skip it
    line = textToBeRead.readLine();
    line = textToBeRead.readLine();
    line = textToBeRead.readLine();
    qDebug() << line;


}

