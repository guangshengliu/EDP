#ifndef LIB_H
#define LIB_H

#include <QScreen>
#include <QGuiApplication>
#include <QTableWidget>


int Get_QScreen_Widget()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.width();
}

int Get_QScreen_Height()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.height();
}

#endif // LIB_H
