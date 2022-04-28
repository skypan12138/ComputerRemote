#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <string>
#include <QKeyEvent>
#include <Windows.h>
#include <QSystemTrayIcon>  //t托盘类
#include <QDesktopServices> //桌面事件类
#include <QAction>
#include <QMenu>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->serialPort = new QSerialPort;
    findFreePorts();

    // Set background picture as bj.jpg
    this->setObjectName("MainWindow");
    this->setStyleSheet("MainWindow{border-image:url(:/Image/background.png);}");

    //Automatically open serial port
    initSerialPort();
    ui->btnSend->setEnabled(true);
    connect(ui->openCom, &QCheckBox::toggled, [=](bool checked){
        if (checked){
            initSerialPort();
            ui->btnSend->setEnabled(true);
        }else{
            this->serialPort->close();
            ui->btnSend->setEnabled(false);
            ui->openCom->setChecked(false);
        }
    });

    connect(this->serialPort, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(ui->btnSend, &QPushButton::clicked, [=](){
        sendMsg(ui->message->toPlainText());
    });


    //==================================================================================
    //***托盘***
    tray= new QSystemTrayIcon(this);//初始化托盘对象tray
    tray->setIcon(QIcon(QPixmap(":/Image/logo.png")));//设定托盘图标，引号内是自定义的png图片路径
    tray->setToolTip("My helper"); //提示文字
    QString title="APP Message";
    QString text="My helper start up";
    tray->show();//让托盘图标显示在系统托盘上
    tray->showMessage(title,text,QSystemTrayIcon::Information,3000); //最后一个参数为提示时长，默认10000，即10s


    //创建菜单项动作
    QAction* minimizeAction = new QAction("MinWin", this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    QAction* maximizeAction = new QAction("MaxWin", this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    QAction* restoreAction = new QAction("restore", this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction* quitAction = new QAction("exit", this);

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用

    //创建托盘菜单
    trayMenu = new QMenu(this);
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(maximizeAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    tray->setContextMenu(trayMenu);

    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //The initializtion is minimal,and it is automatically hidden when computer is turned on.
    setWindowState(Qt::WindowMinimized);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Find free serial port
void MainWindow::findFreePorts(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); ++i){
        if (ports.at(i).isBusy()){
            ports.removeAt(i);
            continue;
        }
        ui->portName->addItem(ports.at(i).portName());
    }
    if (!ports.size()){
        QMessageBox::warning(NULL,"Tip",QStringLiteral("找不到空闲串口"));
        return;
    }
}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason ireason)
{
    switch (ireason)
    {
    case QSystemTrayIcon::Trigger:
        this->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(tray->isVisible())
    {
        hide(); //隐藏窗口
        event->ignore(); //忽略事件
    }
}
void MainWindow::hideEvent(QHideEvent *event)
{
    if(tray->isVisible())
    {
        hide(); //隐藏窗口
        event->ignore(); //忽略事件
    }
}



//initialize the serial port
bool MainWindow::initSerialPort(){
    this->serialPort->setPortName(ui->portName->currentText());
    if (!this->serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::warning(NULL,"Tip",QStringLiteral("串口打开失败"));
        return false;
    }
    this->serialPort->setBaudRate(ui->baudRate->currentText().toInt());

    if (ui->dataBits->currentText().toInt() == 8){
        this->serialPort->setDataBits(QSerialPort::Data8);
    }else if (ui->dataBits->currentText().toInt() == 7){
        this->serialPort->setDataBits(QSerialPort::Data7);
    }else if (ui->dataBits->currentText().toInt() == 6){
        this->serialPort->setDataBits(QSerialPort::Data6);
    }else if (ui->dataBits->currentText().toInt() == 5){
        this->serialPort->setDataBits(QSerialPort::Data5);
    }

    if (ui->stopBits->currentText().toInt() == 1){
        this->serialPort->setStopBits(QSerialPort::OneStop);
    }else if (ui->stopBits->currentText().toInt() == 2){
        this->serialPort->setStopBits(QSerialPort::TwoStop);
    }


    if(ui->parity->currentText() == "NoParity"){
        this->serialPort->setParity(QSerialPort::NoParity);
    }else if (ui->parity->currentText() == "EvenParity"){
        this->serialPort->setParity(QSerialPort::EvenParity);
    }else if (ui->parity->currentText() == "OddParity"){
        this->serialPort->setParity(QSerialPort::OddParity);
    }
    return true;
}
//向串口发送信息
void MainWindow::sendMsg(const QString &msg){
    this->serialPort->write(QByteArray::fromHex(msg.toLatin1()));
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [send] " + msg + "\n");
}
//接受来自串口的信息
void MainWindow::recvMsg(){
    QByteArray msg = this->serialPort->readAll();
    //do something
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [recieve] " + msg.data() + "\n");

    qDebug() << "revieve :"<< msg.data();
    //Only determine the first received character
    char *data=msg.data();
    qDebug() <<"1:"<<data[0]<<"2:"<<data[1]<<"3:"<<data[2]<<"4:"<<data[3];


    //===============================判断接受到哪个字符，然后模拟相关快捷键发送给AHK===============================
    //Key Code ref from https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    //Always like ctrl + alt + LETTER
    if(data[0] == 'W'){
         qDebug() << "successfully recieve!!";

         //按下
         keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
         keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
         keybd_event(0x57,0,0,0);

         //弹起
         keybd_event(0x57,0,2,0);
         keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
         keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
         ui->comLog->insertPlainText(" Successfully Open WeChat \n");

    }
    else if(data[0] == 'O'){
         qDebug() << "successfully recieve!!";

         //按下
         keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
         keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
         keybd_event(0x4f,0,0,0);

         //弹起
         keybd_event(0x4f,0,2,0);
         keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
         keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
         ui->comLog->insertPlainText(" Successfully Open OneNote \n");

    }
    else if(data[0] == 'B'){
         qDebug() << "successfully recieve!!";

         //按下
         keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
         keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
         keybd_event(0x42,0,0,0);

         //弹起
         keybd_event(0x42,0,2,0);
         keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
         keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
         ui->comLog->insertPlainText(" Successfully Open Bilibili \n");

    }
    else if(data[0] == 'L'){
         qDebug() << "successfully recieve!!";

         //按下
         keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
         keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
         keybd_event(0x4c,0,0,0);

         //弹起
         keybd_event(0x4c,0,2,0);
         keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
         keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
         ui->comLog->insertPlainText(" Successfully Open Leetcode \n");

    }
    else if(data[0] == 'M'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
        keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
        keybd_event(0x4d,0,0,0);

        //弹起
        keybd_event(0x4d,0,2,0);
        keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
        keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
        ui->comLog->insertPlainText(" Successfully Open Music \n");

   }
    else if(data[0] == 'S'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
        keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
        keybd_event(0x47,0,0,0);

        //弹起
        keybd_event(0x47,0,2,0);
        keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
        keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
        ui->comLog->insertPlainText(" Successfully Open Music \n");

   }
    else if(data[0] == 'Z'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
        keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
        keybd_event(0x5a,0,0,0);

        //弹起
        keybd_event(0x5a,0,2,0);
        keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
        keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
        ui->comLog->insertPlainText(" Successfully Mute \n");

   }
    else if(data[0] == 'U'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
        keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
        keybd_event(0x55,0,0,0);

        //弹起
        keybd_event(0x55,0,2,0);
        keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
        keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
        ui->comLog->insertPlainText(" Successfully Up Vol \n");

   }
    else if(data[0] == 'D'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(VK_CONTROL,0,0,0);//模拟ctrl按键按下
        keybd_event(VK_MENU,0,0,0); //模拟alt按键按下
        keybd_event(0x44,0,0,0);

        //弹起
        keybd_event(0x44,0,2,0);
        keybd_event(VK_MENU,0,2,0); //模拟alt按键抬起
        keybd_event(VK_CONTROL,0,2,0); //模拟ctrl按键抬起
        ui->comLog->insertPlainText(" Successfully Down Vol \n");

   }
    else if(data[0] == 'P'){
        qDebug() << "successfully recieve!!";

        //按下
        keybd_event(0x20,0,0,0);//模拟tab键被按下

        //弹起
        keybd_event(0x20,0,2,0);//模拟tab键被抬起

        ui->comLog->insertPlainText(" Successfully Press \n");

   }

}
