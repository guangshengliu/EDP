#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //  set Menu
    file_menu = new QMenu;
    file_menu = menuBar()->addMenu(tr("File"));
    new_action = file_menu->addAction(tr("New"));
    open_action = file_menu->addAction(tr("Open"));

    edit_menu = new QMenu;
    edit_menu = menuBar()->addMenu(tr("Edit"));
    orthogonal_action = edit_menu->addAction(tr("Orthogonal experimental design"));
    uniform_action = edit_menu->addAction(tr("Uniform experimental design"));
    connect(orthogonal_action, &QAction::triggered, this, &MainWindow::New_Orthogonal_Dialog);
    connect(uniform_action, &QAction::triggered, this, &MainWindow::New_Uniform_Dialog);


    analysis_menu = new QMenu;
    analysis_menu = menuBar()->addMenu(tr("Analysis"));
    ANOVA_action = analysis_menu->addAction(tr("Analysis of variance"));
    //  Regression analysis
    Regression_action = analysis_menu->addAction(tr("Regression analysis"));

    //  Set TabWidget
    tab_widget = new QTabWidget;
    tab_widget->setTabShape(QTabWidget::Triangular);
    tab_widget->setTabPosition(QTabWidget::North);
    setCentralWidget(tab_widget);

    //  Close TabWidget
    connect(tab_widget, &QTabWidget::tabCloseRequested,this,&MainWindow::Close_TabWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::New_Orthogonal_Dialog()
{
    Orthogonal_Dialog* orth_dialog = new Orthogonal_Dialog(this);
    orth_dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(orth_dialog, &Orthogonal_Dialog::Send_Messages, this, &MainWindow::New_Orthogonal_Widget);
    orth_dialog->exec();
}

void MainWindow::New_Orthogonal_Widget(int factors ,int levels)
{
    Orthogonal_Widget* orthogonal_widget = new Orthogonal_Widget(factors,levels);
    //  Trigger command to analysis data
    connect(ANOVA_action, &QAction::triggered, orthogonal_widget,&Orthogonal_Widget::Set_Result_Widget);
    tab_widget->setTabsClosable(true);
    if(tab_widget->count() == 0)
        TabWidget_count = 0;
    int cur = tab_widget->addTab(orthogonal_widget, QString::asprintf("Orthogonal Experiment %d",++TabWidget_count));
    tab_widget->setCurrentIndex(cur);
    tab_widget->setVisible(true);
}

void MainWindow::Close_TabWidget(int index)
{
    if(tab_widget->count() == 1)
        tab_widget->setVisible(false);
    tab_widget->removeTab(index);
}

void MainWindow::New_Uniform_Dialog()
{
    Uniform_Dialog* uniform_dialog = new Uniform_Dialog(this);
    uniform_dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(uniform_dialog, &Uniform_Dialog::Send_Messages, this, &MainWindow::New_Uniform_Widget);
    uniform_dialog->exec();
}

void MainWindow::New_Uniform_Widget(int factors ,int levels)
{
    Uniform_Widget* uniform_widget = new Uniform_Widget(factors,levels);
    //  Trigger command to analysis data
    connect(Regression_action, &QAction::triggered, uniform_widget,&Uniform_Widget::Set_Result_Widget);
    tab_widget->setTabsClosable(true);
    if(tab_widget->count() == 0)
        TabWidget_count = 0;
    int cur = tab_widget->addTab(uniform_widget, QString::asprintf("Uniform Experiment %d",++TabWidget_count));
    tab_widget->setCurrentIndex(cur);
    tab_widget->setVisible(true);
}
