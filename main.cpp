#include "mainwindow.h"

#include <QApplication>
#include <QTableWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Experimental data processing");
    w.showMaximized();
    return a.exec();
}
