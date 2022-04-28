#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QList>
#include <QMessageBox>
#include <QDateTime>
#include <QSystemTrayIcon>  //t托盘类
#include <QDesktopServices> //桌面事件类

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void findFreePorts();
    bool initSerialPort();
    void sendMsg(const QString &msg);
    void iconActivated(QSystemTrayIcon::ActivationReason ireason);
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);

public slots:
    void recvMsg();

private:
    Ui::MainWindow *ui;

    QSerialPort *serialPort;
    QMenu *trayMenu;//托盘菜单
    QSystemTrayIcon *tray;//托盘图标添加成员
};

#endif // MAINWINDOW_H
