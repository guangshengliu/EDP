#ifndef EXPERIMENT_TABLE_H
#define EXPERIMENT_TABLE_H

#include <QTableWidget>
#include <QMessageBox>
#include "lib/lib.h"

class Experiment_Table :public QWidget
{
    Q_OBJECT

public:
    Experiment_Table(QWidget *parent = nullptr);
    ~Experiment_Table();
    QTableWidget *experiment_table;

private slots:
    void TableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void TableWidget_itemChanged(QTableWidgetItem *item);

private:
    QString old_text;

};

#endif // EXPERIMENT_TABLE_H
