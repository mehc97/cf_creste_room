#include "workthread.h"
#include "qapplication.h"
#include <QDebug>
#include <QDesktopWidget>

workthread::workthread(QThread *parent)
    : QThread{parent}
{

}

void workthread::changed(HWND mhwnd, int flag)
{
    hwnd = mhwnd;
    tmpFlag = flag;
}

void workthread::lastChange(int flag)
{
    tmpFlag = flag;
}

int workthread::chuli(Mat matSrc1, Mat matStrc2)
{
    Mat matDst1, matDst2;
        // 缩小尺寸：将图像缩小到8*8的尺寸，总共64个像素。这一步的作用是去除图像的细节，只保留结构/明暗等基本信息，摒弃不同尺寸/比例带来的图像差异；
    cv::resize(matSrc1, matDst1, Size(8, 8));
    cv::resize(matStrc2, matDst2, Size(8, 8));
    //灰度化
    cvtColor(matDst1, matDst1, COLOR_BGR2GRAY);
    cvtColor(matDst2, matDst2, COLOR_BGR2GRAY);
    int iAvg1 = 0, iAvg2 = 0;
    int arr1[64], arr2[64];
    for (int i = 0; i < 8; i++) {
        //位置是第i行的头指针
        uchar* data1 = matDst1.ptr<uchar>(i);
        uchar* data2 = matDst2.ptr<uchar>(i);
        int tmp = i * 8;
        for (int j = 0; j < 8; j++) {
            int tmp1 = tmp + j;
            arr1[tmp1] = data1[j] / 4 ;
            arr2[tmp1] = data2[j] / 4 ;
            iAvg1 += arr1[tmp1];
            iAvg2 += arr2[tmp1];
        }
    }
    iAvg1 /= 64;
    iAvg2 /= 64;
    for (int i = 0; i < 64; i++) {
        arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
        arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
    }
    int iDiffNum = 0;
    for (int i = 0; i < 64; i++)
        if (arr1[i] != arr2[i])
            ++iDiffNum;
    return iDiffNum;
}

//void workthread::panDuanimg()
//{
//        Mat img1=imread("C:/Users/xiaohan/Desktop/duibi3.jpg");
//        Mat img2=imread("C:/Users/xiaohan/Desktop/RoomJi.png");

//        int den=chuli(img1, img2);
//        if (den <= 5)
//        {
//            panduan = "They are too similar!";
//        }
//        else if (den > 10)
//        {
//            panduan = "They are two different pictures!";
//        }
//        else
//        {
//            panduan = "They are a litter similar.";
//        }
//        den = 100 - den;
//        QString str = QString::number(den);
//        QString buf = "相似度为： " + str +"%  我的判断为： " + panduan;

//        qDebug()<<buf;

//}

void workthread::run()
{

    while(tmpFlag != 0)
    {
        //移动窗口位置
        Sleep(1000);
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        //移动 到 创建房间
        Sleep(1000);
        SetCursorPos(498,518);

        Sleep(1000);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        //选择挑战模式
        Sleep(1000);
        SetCursorPos(327,201);

        Sleep(300);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        //设置房间名
        Sleep(1000);
        SetCursorPos(731,204);

        Sleep(300);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        //粘贴房间名
        Sleep(1000);
        keybd_event(17,0,0,0);
        keybd_event(86,0,0,0);
        keybd_event(17,0,KEYEVENTF_KEYUP,0);
        keybd_event(86,0,KEYEVENTF_KEYUP,0);

        //移动到确定创建位置
        Sleep(1000);
        SetCursorPos(1003,627);

        Sleep(300);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        //如果房间满了，判断，等待几秒重新执行
        Sleep(1000);
        //截图
        QDesktopWidget *pDesk = QApplication::desktop();
        QScreen *screen = QGuiApplication::primaryScreen();
        QPixmap RoomPic = screen->grabWindow(pDesk->winId(),409,251,467,218);

        QFile file("hell.jpg");
        if(file.exists())
        {
            file.remove();
        }
        RoomPic.save("hell.jpg");

        Sleep(500);
        Mat img1=imread("./hell.jpg");
        Mat img2=imread("./RoomJi.png");

        int den=chuli(img1, img2);
        if(den <= 8)
        {
            Sleep(500);
            SetCursorPos(635,434);
            Sleep(300);
            mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

            //点击确定后发送信号给程序，等待几秒后再重新创建
            emit RoomMan();

            Sleep(5000);

            continue;

        }

        //没有窗口就继续后面操作

//        Sleep(1000);
//        SetCursorPos(1174,525);
//        Sleep(300);
//        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

//        Sleep(1000);
//        SetCursorPos(711,697);
//        Sleep(300);
//        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


        Sleep(1000);
        SetCursorPos(1169,525);
        Sleep(300);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        emit JoinOk();

        //按下退出
        Sleep(180000);
        keybd_event(27,0,0,0);
        keybd_event(27,0,KEYEVENTF_KEYUP,0);


        //按两次enter
        Sleep(1000);
        keybd_event(13,0,0,0);
        keybd_event(13,0,KEYEVENTF_KEYUP,0);

        Sleep(1000);
        keybd_event(13,0,0,0);
        keybd_event(13,0,KEYEVENTF_KEYUP,0);

        //        Sleep(1000);
        //        keybd_event(13,0,0,0);
        //        keybd_event(13,0,KEYEVENTF_KEYUP,0);


        Sleep(1000);
        SetCursorPos(1162,651);
        Sleep(500);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        Sleep(1000);
        SetCursorPos(990,641);
        Sleep(500);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

        //成为房主框
        Sleep(1000);
        SetCursorPos(683,438);
        Sleep(500);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);



        Sleep(1000);
        SetCursorPos(1141,20);
        Sleep(500);
        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);



//        Sleep(1000);
//        SetCursorPos(713,695);
//        Sleep(500);
//        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

//        Sleep(1000);
//        SetCursorPos(1118,253);
//        Sleep(500);
//        mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);



        times ++;
        emit sendTimes(times);

        Sleep(5000);



    }
}
