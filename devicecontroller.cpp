#include "devicecontroller.h"
#include <QMutexLocker>
#include <QDebug>

DeviceController::DeviceController()
{
}

DeviceController::~DeviceController() {

}

void DeviceController::setTargetTemp(float temp){
    QMutexLocker locker(&_mutex);
    _targetTemp = temp;
}

float DeviceController::getTargetTemp() {
    QMutexLocker locker(&_mutex);
    return _targetTemp;
}

void DeviceController::setRealTemp(int id, float temp) {
    emit realTempChanged(id, temp);
}

void DeviceController::heating(int id, bool heating) {
    emit heatingStatusChanged(id, heating);
}
