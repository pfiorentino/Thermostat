#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QList>

#include "device.h"

#include "ui_device.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void realTempChanged(int id, float temp);
    void heatingStatusChanged(int id, bool heating);

    void on_randomDevices_clicked();

private:
    void setTargetTemp(float targetTemp);
    Ui::MainWindow *ui;
    typedef std::unique_ptr<Device> DevicePtr;
    DeviceController _devCtrl;
    std::list<DevicePtr> _devices;
    QList<QLCDNumber*> _devicesDisplay;
    QList<Ui::Device*> _devicesUi;
};

#endif // MAINWINDOW_H
