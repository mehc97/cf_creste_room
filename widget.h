#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <WinUser.h>
#include <QThread>
#include "workthread.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFile>
#include <vector>
#include <opencv2/opencv.hpp>
#include<QGraphicsScene>
#include<QGraphicsView>                   //graphicsview类
#include <string>
using namespace cv;


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int tmpFlag = 0;
    workthread *work ;



    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_putName_clicked();

signals:
    void sendWork( HWND hwnd,int flag);
    void changeFlag(int flag);
    void endWork(int flag);

private:
    Ui::Widget *ui;


};
#endif // WIDGET_H
