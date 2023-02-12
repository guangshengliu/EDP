#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void New_Orthogonal_Widget(int factors ,int levels);

protected:
    QMenuBar *menu_bar;

    QAction *open_action;  //定义一个动作
    QMenu *file_menu;  //定义一个菜单
    QAction *new_action;
    QMenu *edit_menu;

    QMenu *new_menu;    // New下二级菜单
    QAction *orthogonal_action;   // 正交实验
    QAction *uniform_action;   // 均匀实验

    QTabWidget *tab_widget;
    QWidget *orthogonal_widget;
    int TabWidget_count = 0;

    QStatusBar *status_bar;

private slots:
    void New_Orthogonal_Dialog();
    void Close_TabWidget(int index);
};

#endif // MAINWINDOW_H
