#ifndef ORTHOGONAL_WIDGET_H
#define ORTHOGONAL_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QChartView>
#include <QScatterSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QTextDocumentFragment>
#include "orthogonal_result.h"
#include "../experiment_table.h"

class Orthogonal_Widget : public QWidget
{
    Q_OBJECT

public:
    Orthogonal_Widget(int factors ,int levels, QWidget *parent = nullptr);
    ~Orthogonal_Widget();

    void Select_Orth_Table(int factors ,int levels);

    void Set_Result_Widget();
    QVector<int> info_table;
    void Keep_TableWidget_Info(QVector<int> &info_table ,int levels);

    void Draw_Scatter_Series(QVector<double> result);

    void Clear_table(QVector<QTextTable*> table);

private:
    Experiment_Table *orthogonal_table;
    QTableWidget *table_widget;
    Orthogonal_Result *result_widget;

    QHBoxLayout *h_layout;
    QSplitter *h_splitter;

};

#endif // ORTHOGONAL_WIDGET_H
