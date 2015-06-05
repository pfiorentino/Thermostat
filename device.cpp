#include "device.h"
#include <QDebug>

Device::Device(const int tNum, DeviceController &ctrl):
    _ctrl(ctrl), _tNum(tNum)
{
    randomRealTemp();
}

void Device::run() {
    while(_stopSig == false){
        heatingOrNot();

        QString hStr;
        if (_heating == true){
            hStr = "Heating";
            _currentTemp += 0.01;
        } else {
            hStr = "Not Heating";
        }

        qDebug() << "Device" << _tNum << "(" << _cycle << "):" << hStr << _currentTemp << "°C to " << _desiredTemp << "°C";

        _ctrl.setRealTemp(_tNum, _currentTemp);

        msleep(20);
    }

    qDebug() << "Device" << _tNum << "terminated";
}

void Device::stop() {
    qDebug() << "Device" << _tNum << "stopping...";
    _stopSig = true;
}

void Device::heatingOrNot() {
    _desiredTemp = _ctrl.getTargetTemp();

    if (_cycle == 0) {
        _d = 10*(_desiredTemp-_currentTemp);
        if (_d > 100)
            _d = 100;
        if (_d < 0)
            _d = 0;
    }

    if (_cycle < _d) {
        if (_heating == false)
            _ctrl.heating(_tNum, true);
        _heating = true;
    } else {
        if (_heating == true)
            _ctrl.heating(_tNum, false);
        _heating = false;
    }

    ++_cycle;

    if (_cycle == 100)
        _cycle = 0;
}

void Device::randomRealTemp() {
    _currentTemp = qrand() % 20 + 10;
    _cycle = 0;
}
