#ifndef THREADCLASS_H
#define THREADCLASS_H

#include <QObject>
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
#include <Windows.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <WinUser.h>
class threadClass : public QObject
{
    Q_OBJECT
public:
    explicit threadClass(QObject *parent = nullptr);

    int tmpFlag = 0;
signals:

};

#endif // THREADCLASS_H
