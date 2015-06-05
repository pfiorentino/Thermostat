#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QMutex>

class DeviceController : public QObject
{
    Q_OBJECT

public:
    DeviceController();
    ~DeviceController();

    void setTargetTemp(float temp);
    float getTargetTemp();
    void setRealTemp(int id, float temp);
    void heating(int id, bool heating);

private:
    QMutex _mutex;
    float _targetTemp;

signals:
    void realTempChanged(int id, float temp);
    void heatingStatusChanged(int id, bool heating);
};

#endif // DEVICECONTROLLER_H
