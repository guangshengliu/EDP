#include "experiment_table.h"

Experiment_Table::Experiment_Table(QWidget *parent)
{
    experiment_table = new QTableWidget;
    experiment_table->setFrameShape(QFrame::Panel);

    // Limit input as number
    connect(experiment_table,&QTableWidget::itemDoubleClicked,this,&Experiment_Table::TableWidget_itemDoubleClicked);
    connect(experiment_table,&QTableWidget::itemChanged,this,&Experiment_Table::TableWidget_itemChanged);
}

Experiment_Table::~Experiment_Table()
{
}

// Get QTableWidget_item text
void Experiment_Table::TableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    old_text = item->text();
}

void Experiment_Table::TableWidget_itemChanged(QTableWidgetItem *item)
{
    // If not number or old_text,recover it
    if(!Judge_Number(item->text()))
    {
       QMessageBox::critical(this,tr("critical"),tr("Please express the factor's level in numbers!"));
       item->setText(old_text);
    }
}
