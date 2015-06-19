#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    int _numThreads = 4;
    ui->setupUi(this);
    setTargetTemp(25);
    ui->desiredTempBox->setValue(25);

    connect(&_devCtrl ,SIGNAL(realTempChanged(int, float)),this,SLOT(realTempChanged(int, float)));
    connect(&_devCtrl ,SIGNAL(heatingStatusChanged(int, bool)),this,SLOT(heatingStatusChanged(int, bool)));

    qDebug() << endl << "-> Launching devices...";

    QWidget *widget = new QWidget;
    Ui::Device *devUi = new Ui::Device();
    devUi->setupUi(widget);
    devUi->deviceName->setText(QString("RealDevice %1").arg(0));
    _devicesUi.push_back(devUi);
    ui->devicesTempLayout->addWidget(widget);
    auto ptr = new RealDevice(0, _devCtrl, "/dev/tty.usbserial-A5026045");
    _devices.push_back(DevicePtr(ptr));
    _devices.back()->start();
    qDebug() << "RealDevice 0 launched";

    for(int i = 1; i < _numThreads; ++i) {
        QWidget *widget = new QWidget;
        Ui::Device *devUi = new Ui::Device();
        devUi->setupUi(widget);
        devUi->deviceName->setText(QString("Capteur %1").arg(i));
        _devicesUi.push_back(devUi);
        ui->devicesTempLayout->addWidget(widget);
        auto ptr = new Device(i, _devCtrl);
        _devices.push_back(DevicePtr(ptr));
        _devices.back()->start();
        qDebug() << "Device" << i << "launched";
    }
}

MainWindow::~MainWindow()
{
    while(_devices.empty() == false) {
        _devices.front()->stop();
        _devices.front()->wait();
        _devices.pop_front();
    }

    delete ui;
}

void MainWindow::setTargetTemp(float targetTemp) {
    _devCtrl.setTargetTemp(targetTemp);
    ui->targetTempBox->display(targetTemp);
}

void MainWindow::on_pushButton_clicked() {
    setTargetTemp(ui->desiredTempBox->value());
}

void MainWindow::realTempChanged(int id, float temp)
{
    _devicesUi.at(id)->lcdNumber->display(temp);
}

void MainWindow::heatingStatusChanged(int id, bool heating){
    if (heating == true){
        _devicesUi.at(id)->heatingIcon->setPixmap(QPixmap("://resources/images/heating_32x32.png"));
    } else {
        _devicesUi.at(id)->heatingIcon->setPixmap(QPixmap("://resources/images/heating_disabled_32x32.png"));
    }
}

void MainWindow::on_randomDevices_clicked()
{
    for (std::list<DevicePtr>::const_iterator iterator = _devices.begin(), end = _devices.end(); iterator != end; ++iterator) {
        if (!(*iterator)->isRealDevice){
            (*iterator)->randomRealTemp();
        }
    }
}
