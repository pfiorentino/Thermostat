#include "realdevice.h"
#include <QDebug>
#include <QMessageBox>

RealDevice::RealDevice(const int tNum, DeviceController &ctrl, QString devPort):
    Device(tNum, ctrl)
{
    isRealDevice = true;
    _initPort(devPort);
}

void RealDevice::run() {
    connect(_port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    while(_stopSig == false){
        float oldDesiredTemp = _desiredTemp;
        _desiredTemp = _ctrl.getTargetTemp();

        if (oldDesiredTemp != _desiredTemp){
            _transmitToDevice(_desiredTemp);
        }

        msleep(20);
    }

    qDebug() << "RealDevice" << _tNum << "terminated";
}

void RealDevice::stop() {
    qDebug() << "RealDevice" << _tNum << "stopping...";
    _stopSig = true;
}

void RealDevice::_initPort(QString devPort) {
    _port = new QextSerialPort(devPort);

    _port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    if(!_port->isOpen()) {
        qDebug() << "Impossible d'ouvrir le port" << devPort;
    }

    _port->setBaudRate(BAUD9600);
    _port->setFlowControl(FLOW_OFF);
    _port->setParity(PAR_NONE);
    _port->setDataBits(DATA_8);
    _port->setStopBits(STOP_1);
}

void RealDevice::onReadyRead() {
    _getFromDevice();
}

void RealDevice::_getFromDevice()
{
    float oldTemp = _currentTemp;
    bool oldHeating = _heating;

    if(_port != NULL && _port->isOpen()){
        qDebug() << "Byte available: " << _port->bytesAvailable();

        // On ne charge pas si le buffer n'est pas complet
        if (_port->bytesAvailable() == 3){
            char* byte = new char[_port->bytesAvailable()];
            _port->read(byte, _port->bytesAvailable());

            _currentTemp = (int) byte[1];
            _heating = (int) byte[0] == 2;

            qDebug() << "Value read:" << (int) byte[0] << "";
            qDebug() << "Target temp:" << (int) byte[2] << "";
        }

        _port->flush(); // on attend la fin de la transmission
    }

    if (oldTemp != _currentTemp){
        _ctrl.setRealTemp(_tNum, _currentTemp);
    }

    if (oldHeating != _heating){
        _ctrl.heating(_tNum, _heating);
    }
}

void RealDevice::_transmitToDevice(int value)
{
    char* byte = new char[1];
    byte[0] = value;
    if(_port != NULL){
        qint64 bw = _port->write(byte);
        qDebug() << bw << "byte(s) written | Value sent:" << value << "(dec)";
        _port->flush();
    }

}
