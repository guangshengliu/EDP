#include "orthogonal_widget.h"
#include "orthogonal_table.h"
#include "../lib/lib.h"

Orthogonal_Widget::Orthogonal_Widget(int factors ,int levels, QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    table_widget = new QTableWidget;
    table_widget->setFrameShape(QFrame::Panel);

    QHBoxLayout *h_layout = new QHBoxLayout(this);
    // Left is tablewidget,right is reslutwidget
    QSplitter *h_splitter = new QSplitter(this);
    h_splitter->setStyleSheet("QSplitter:handle{background-color:grey}");
    reslut_widget = new QWidget;
    h_splitter->addWidget(table_widget);
    h_splitter->addWidget(reslut_widget);
    // left:right = 3:1
    h_splitter->setStretchFactor(0,3);
    h_splitter->setStretchFactor(1,1);

    h_layout->addWidget(h_splitter);

    Select_Orth_Table(factors,levels);

    QList<QString> title;
    for (int var = 1; var < factors + 1; ++var)
        title.append(QString::asprintf("Factor %d",var));
    table_widget->setHorizontalHeaderLabels(title);

    // Limit input as number
    connect(table_widget,&QTableWidget::itemDoubleClicked,this,&Orthogonal_Widget::TableWidget_itemDoubleClicked);
    connect(table_widget,&QTableWidget::itemChanged,this,&Orthogonal_Widget::TableWidget_itemChanged);
}

Orthogonal_Widget::Orthogonal_Widget(QWidget *parent)
    :QWidget(parent)
{
}

Orthogonal_Widget::~Orthogonal_Widget()
{
}

void Orthogonal_Widget::Select_Orth_Table(int factors ,int levels)
{
    if(levels == 2)
    {
        if(factors <= 3 && factors > 0)
            Orth_Table_3_2(table_widget,factors);
        else if(factors > 3 && factors <= 7)
            Orth_Table_7_2(table_widget,factors);
        else if(factors > 7 && factors <= 11)
            Orth_Table_11_2(table_widget,factors);
        else
            return ;
    }
    else if(levels == 3)
    {
        if(factors <= 4 && factors > 0)
            Orth_Table_4_3(table_widget,factors);
    }
    else if(levels == 4)
    {
        if(factors <= 5 && factors > 0)
            Orth_Table_5_4(table_widget,factors);
    }
    else if(levels == 5)
    {
        if(factors <= 6 && factors > 0)
            Orth_Table_6_5(table_widget,factors);
    }
}

// Get QTableWidget_item text
void Orthogonal_Widget::TableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    old_text = item->text();
}

void Orthogonal_Widget::TableWidget_itemChanged(QTableWidgetItem *item)
{
    // If not number or old_text,recover it
    if(!Judge_Number(item->text()))
    {
       QMessageBox::critical(this,tr("critical"),tr("Please express the factor's level in numbers!"));
       item->setText(old_text);
    }
}

void Orthogonal_Widget::Set_Result_Widget()
{
    if(!Judge_QTablewidget(table_widget))
        QMessageBox::critical(this,tr("critical"),tr("Please ensure that the data has been entered completely!"));
    ANOVA(table_widget);
}
