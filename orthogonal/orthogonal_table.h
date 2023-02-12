#ifndef ORTHOGONAL_TABLE_H
#define ORTHOGONAL_TABLE_H

#include <QTableWidget>

//  factors:3    levels:2
void Orth_Table_3_2(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(4);
    table_widget->setColumnCount(factor);

    int result[4][3] = {1,1,1,
                      1,2,2,
                      2,1,2,
                      2,2,1};
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            // set text in the center
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//  factors:7    levels:2
void Orth_Table_7_2(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(8);
    table_widget->setColumnCount(factor);
    int result[8][7] = {1,1,1,1,1,1,1,
                      1,1,1,2,2,2,2,
                      1,2,2,1,1,2,2,
                      1,2,2,2,2,1,1,
                      2,1,2,1,2,1,2,
                      2,1,2,2,1,2,1,
                      2,2,1,1,2,2,1,
                      2,2,1,2,1,1,2};
    for (int i = 0; i < 8; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//  factors:11    levels:2
void Orth_Table_11_2(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(12);
    table_widget->setColumnCount(factor);
    int result[12][11] = {1,1,1,1,1,1,1,1,1,1,1,
                      1,1,1,1,1,2,2,2,2,2,2,
                      1,1,2,2,2,1,1,1,2,2,2,
                      1,2,1,2,2,1,2,2,1,1,2,
                      1,2,2,1,2,2,1,2,1,2,1,
                      1,2,2,2,1,2,2,1,2,1,1,
                      2,1,2,2,1,1,2,2,1,2,1,
                      2,1,2,1,2,2,2,1,1,1,2,
                      2,1,1,2,2,2,1,2,2,1,1,
                      2,2,2,1,1,1,1,2,2,1,2,
                      2,2,1,2,1,2,1,1,1,2,2,
                      2,2,1,1,2,1,2,1,2,2,1};
    for (int i = 0; i < 12; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//  factors:4    levels:3
void Orth_Table_4_3(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(9);
    table_widget->setColumnCount(factor);
    int result[9][4] = {1,1,1,1,
                      1,2,2,2,
                      1,3,3,3,
                      2,1,2,3,
                      2,2,3,1,
                      2,3,1,2,
                      3,1,3,2,
                      3,2,1,3,
                      3,3,2,1};
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//  factors:5    levels:4
void Orth_Table_5_4(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(16);
    table_widget->setColumnCount(factor);
    int result[16][5] = {1,1,1,1,1,
                      1,2,2,2,2,
                      1,3,3,3,3,
                      1,4,4,4,4,
                      2,1,2,3,4,
                      2,2,1,4,3,
                      2,3,4,1,2,
                      2,4,3,2,1,
                      3,1,3,4,2,
                      3,2,4,3,1,
                      3,3,1,2,4,
                      3,4,2,1,3,
                      4,1,4,2,3,
                      4,2,3,1,4,
                      4,3,2,4,1,
                      4,4,1,3,2};
    for (int i = 0; i < 16; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

//  factors:6    levels:5
void Orth_Table_6_5(QTableWidget *table_widget ,int factor)
{
    table_widget->setRowCount(25);
    table_widget->setColumnCount(factor);
    int result[25][6] = {1,1,1,1,1,1,
                      1,2,2,2,2,2,
                      1,3,3,3,3,3,
                      1,4,4,4,4,4,
                      1,5,5,5,5,5,
                      2,1,2,3,4,5,
                      2,2,3,4,5,1,
                      2,3,4,5,1,2,
                      2,4,5,1,2,3,
                      2,5,1,2,3,4,
                      3,1,3,5,2,4,
                      3,2,4,1,3,5,
                      3,3,5,2,4,1,
                      3,4,1,3,5,2,
                      3,5,2,4,1,3,
                      4,1,4,2,5,3,
                      4,2,5,3,1,4,
                      4,3,1,4,2,5,
                      4,4,2,5,3,1,
                      4,5,3,1,4,2,
                      5,1,5,4,3,2,
                      5,2,1,5,4,3,
                      5,3,2,1,5,4,
                      5,4,3,2,1,5,
                      5,5,4,3,2,1};
    for (int i = 0; i < 25; ++i){
        for (int j = 0; j < factor; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

#endif // ORTHOGONAL_TABLE_H
