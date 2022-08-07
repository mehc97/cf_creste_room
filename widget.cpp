#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QTextCodec>
#include <QRect>
#include <stdio.h>
#include <iostream>
#include <QTextStream>
#include <QKeySequence>

#pragma comment  (lib, "User32.lib")
ATOM HTK_RUN;
ATOM HTK_END;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置显示中文
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
    QTextCodec::setCodecForLocale(codec);

    //固定窗口
    setWindowTitle("仅供测试使用");
    this->setFixedSize(335,337);
    ui->textBrowser->setReadOnly(true);

    // 1 获取热键标识

    HTK_RUN = GlobalAddAtomA("HTK_RUN");

    HTK_END = GlobalAddAtomA("HTK_END");

    // 2 注册全局热键

    RegisterHotKey(HWND(this->winId()), HTK_RUN, MOD_CONTROL, 'Q');    // ctrl+

    RegisterHotKey(HWND(this->winId()), HTK_END, MOD_CONTROL, 'W');    // ctrl+

    work = new workthread;

    connect(this,&Widget::sendWork,work,&workthread::changed);
    connect(this,&Widget::endWork,work, &workthread::lastChange);
    connect(work,&workthread::sendTimes,this,[=](int num){

        ui->textBrowser->append(tr("成功创建了%1次房间").arg(num));

    });
    connect(work,&workthread::RoomMan,this,[=](){
        ui->textBrowser->append(tr("检测到房间已满，等待5秒后继续创建......"));
    });
    connect(work,&workthread::JoinOk,this,[=](){
        ui->textBrowser->append(tr("正在进入游戏......"));
    });

}

Widget::~Widget()
{
    // 3 销毁全局热键

    UnregisterHotKey(HWND(this->winId()), HTK_RUN);

    UnregisterHotKey(HWND(this->winId()), HTK_END);

    // 4 释放热键标识

    GlobalDeleteAtom(HTK_RUN);

    GlobalDeleteAtom(HTK_END);


    delete ui;
}



bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "windows_generic_MSG")
       {
           MSG* pMsg = reinterpret_cast<MSG*>(message);
           if(pMsg->message == WM_HOTKEY)
           {
               if(pMsg->wParam == HTK_RUN)
               {
                    ui->pushButton_Start->click();
               }
               else if(pMsg->wParam == HTK_END)
               {
                    ui->pushButton_Stop->click();
               }

           }
       }
    return QWidget::nativeEvent(eventType,message,result);

}



void Widget::on_pushButton_Start_clicked()
{

    ui->pushButton_Start->setEnabled(false);

    tmpFlag = 1;


    ui->textBrowser->append(tr("正在检测游戏是否运行，请稍后......"));

    // 判断进程是否存在
    QProcess tasklist;
    tasklist.start("tasklist.exe",QStringList()<<"/FI"<<QString("imagename eq %1").arg("crossfire.exe"));

    tasklist.waitForFinished();

    QByteArray outputStr = tasklist.readAllStandardOutput();
    QString resultStr = QString::fromLocal8Bit(outputStr);
   // qDebug()<<resultStr;

    if(resultStr.contains("crossfire.exe"))
    {
        ui->textBrowser->append(tr("已检测到游戏运行，执行软件中......"));

        //QT需要管理员运行，才能控制游戏窗口

        QString name = "穿越火线";
        //QString leiName = "Chrome_RenderWidgetHostHWND";
        //LPCWSTR gamename = reinterpret_cast<LPCWSTR>(leiName.data());

        //查找窗口句柄（这里的参数记得是宽字符形式，用LPCWSTR转换或者直接填入L" "）
        HWND hwnd = FindWindow(NULL, LPCWSTR(name.unicode()));
        if(hwnd) //句柄是否存在
        {
            qDebug()<<"hw"<<hwnd;

            RECT rect;
            GetWindowRect(hwnd,&rect);

            int w = rect.right-rect.left;
            int h = rect.bottom-rect.top;

            qDebug()<<"宽:"<<w<<" "<<"高:"<<h;
            emit sendWork(hwnd,tmpFlag);
            work->start();

        }


    }
    else
    {
        ui->textBrowser->append("未检测到游戏运行，软件已停止");
        ui->pushButton_Start->setEnabled(true);
    }
    //return outputStr.contains("PID") && outputStr.contains(exeName);


}


void Widget::on_pushButton_Stop_clicked()
{
    ui->pushButton_Start->setEnabled(true);
    ui->textBrowser->append("软件已停止");

    tmpFlag = 0;

    //emit endWork(tmpFlag);
    work->terminate();
    work->wait();

}


void Widget::on_pushButton_putName_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QString fuzhistr = ui->lineEdit_RoomName->text();
    QByteArray bystr = fuzhistr.toLocal8Bit();
    char * chstr;
    chstr = bystr.data();


    HWND hWnd = NULL;
    OpenClipboard(hWnd);//打开剪切板
    EmptyClipboard();//清空剪切板
    HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);//分配内存
    char* pData = (char*)GlobalLock(hHandle);//锁定内存，返回申请内存的首地址
    strcpy(pData,chstr);//或strcpy(pData, "被复制的内容");
    //另外被复制的内容为char *类型
    SetClipboardData(CF_TEXT, hHandle);//设置剪切板数据
    GlobalUnlock(hHandle);//解除锁定
    CloseClipboard();//关闭剪切板
}
