#include "orthogonal_widget.h"
#include "orthogonal_table.h"

Orthogonal_Widget::Orthogonal_Widget(int factors ,int levels, QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    table_widget = new QTableWidget;
    table_widget->setFrameShape(QFrame::Panel);

    QHBoxLayout *h_layout = new QHBoxLayout(this);
    h_layout->addWidget(table_widget);
    Select_Orth_Table(factors,levels);

    QList<QString> title;
    for (int var = 1; var < factors + 1; ++var)
        title.append(QString::asprintf("Factor %d",var));
    table_widget->setHorizontalHeaderLabels(title);
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
