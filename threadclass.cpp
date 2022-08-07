#include "threadclass.h"

threadClass::threadClass(QObject *parent)
    : QObject{parent}
{

}
void threadClass::CreateRooms(HWND hwnd, int flag)
{
    qDebug()<<"jinlaile";

    tmpFlag = flag;




}

void threadClass::changeFlag(int flag)
{
    tmpFlag = flag;

}
