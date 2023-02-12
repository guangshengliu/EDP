#include "orthogonal_dialog.h"

Orthogonal_Dialog::Orthogonal_Dialog(QWidget *parent)
    :QDialog(parent)
{
    QLabel *text1 = new QLabel("factors:");
    QLabel *text2 = new QLabel("levels :");

    factors = new QComboBox;
    levels = new QComboBox;
    for (int var = 2; var < 12; ++var)
        factors->addItem(QString::number(var));

    for (int var = 2; var < 6; ++var)
        levels->addItem(QString::number(var));

    confirm = new QPushButton("confirm");
    cancel = new QPushButton("cancel");
    connect(confirm,&QPushButton::clicked,this,&Orthogonal_Dialog::confirm_clicked);
    connect(cancel,&QPushButton::clicked,this,&Orthogonal_Dialog::close);

    gridlayout = new QGridLayout(this);
    gridlayout->setColumnMinimumWidth(0,50);
    gridlayout->setRowMinimumHeight(0,10);
    gridlayout->setColumnMinimumWidth(8,50);
    gridlayout->setRowMinimumHeight(5,10);

    gridlayout->addWidget(text1,1,1,1,3);
    gridlayout->addWidget(text2,1,5,1,3);
    gridlayout->setColumnMinimumWidth(4,20);

    gridlayout->addWidget(factors,2,1,1,3);
    gridlayout->addWidget(levels,2,5,1,3);
    gridlayout->setRowMinimumHeight(3,30);
    gridlayout->addWidget(confirm,4,2,1,1);
    gridlayout->addWidget(cancel,4,6,1,1);
}

Orthogonal_Dialog::~Orthogonal_Dialog()
{
}

void Orthogonal_Dialog::confirm_clicked()
{
    int Factor = factors->currentText().toInt();
    int Level = levels->currentText().toInt();
    emit Send_Messages(Factor,Level);
    this->close();
}
