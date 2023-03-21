#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "orthogonal/orthogonal_dialog.h"
#include "orthogonal/orthogonal_widget.h"
#include "uniform/uniform_dialog.h"
#include "uniform/uniform_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void New_Orthogonal_Widget(int factors ,int levels);
    void New_Uniform_Widget(int factors ,int levels);

protected:
    QMenuBar *menu_bar;

    QAction *open_action;
    QMenu *file_menu;
    QAction *new_action;
    QMenu *edit_menu;

    QMenu *new_menu;
    QAction *orthogonal_action;
    QAction *uniform_action;

    QMenu *analysis_menu;
    QAction *ANOVA_action;
    QAction *Regression_action;

    QTabWidget *tab_widget;
    int TabWidget_count = 0;

    Orthogonal_Widget *orthogonal_widget;
    Orthogonal_Dialog *orth_dialog;
    Uniform_Dialog *uniform_dialog;

private slots:
    void New_Orthogonal_Dialog();
    void New_Uniform_Dialog();

    void Close_TabWidget(int index);

};

#endif // MAINWINDOW_H
