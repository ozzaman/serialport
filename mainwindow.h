#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialportcontroller.h>
#include <QTimer>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialportController *goSerialPort = new SerialportController(this);

public slots:

    void listAvailablePorts();
    void populatePortsCombo(QString);
    void populateBaudRateCombo(int);

private slots:

    void onwriteToPortBtnClicked();

    void onportsComboActivated(int);
    void readReceviedPortData();
    void onbaudRatesComboCurrentIndexChanged(int index);
    void closeSerialPort();

    void onserialPortCntBtnClicked();
    void fileOpen();

    void onreadFileBtnClicked();

private:
    QString currentRequest;
    QList<qint32> availableBaudRatesList;
    QList<QString> portsList;

};
#endif // MAINWINDOW_H
