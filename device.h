#ifndef DEVICE_H
#define DEVICE_H

#include <QThread>

#include "devicecontroller.h"

class Device : public QThread
{
public:
    Device(const int tNum, DeviceController &ctrl);
    virtual void run();
    void stop();
    void heatingOrNot();
    void randomRealTemp();
private:
    DeviceController &_ctrl;

    int _d;
    int _cycle;
    int _tNum;
    float _desiredTemp = -1;
    float _currentTemp;
    bool _heating = false;
    bool _stopSig = false;
};

#endif // DEVICE_H
