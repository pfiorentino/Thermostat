#ifndef REALDEVICE_H
#define REALDEVICE_H

#include "device.h"
#include "devicecontroller.h"
#include "qextserialport.h"

class RealDevice : public Device
{
    Q_OBJECT

    public:
        RealDevice(const int tNum, DeviceController &ctrl, QString devPort);
        virtual void run();
        void stop();
    private slots:
        void onReadyRead();
    private:
        void _initPort(QString devPort);
        void _transmitToDevice(int value);
        void _getFromDevice();

        QextSerialPort *_port;
};

#endif // REALDEVICE_H
