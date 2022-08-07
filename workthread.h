#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <Windows.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <WinUser.h>
#include <QMutex>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFile>
#include <vector>
#include <opencv2/opencv.hpp>
#include<QGraphicsScene>
#include<QGraphicsView>                   //graphicsview类
#include <string>
#include <QFile>

using namespace cv;

class workthread : public QThread
{
    Q_OBJECT
public:
    explicit workthread(QThread *parent = nullptr);
    int tmpFlag = 0;
    HWND hwnd;
    QMutex pause;

    int times = 0;

    void changed(HWND mhwnd, int flag);
    void lastChange(int flag);
//    void panDuanimg();
    int chuli(Mat matSrc1, Mat matStrc2);

protected:
    void run();

signals:
    void sendTimes(int num);
    void RoomMan();
    void JoinOk();

private:
    int iDiffNum;
};

#endif // WORKTHREAD_H
