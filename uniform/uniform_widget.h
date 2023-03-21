#ifndef UNIFORM_WIDGET_H
#define UNIFORM_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QChartView>
#include <QScatterSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QTextDocumentFragment>
#include "../experiment_table.h"
#include "uniform_result.h"

class Uniform_Widget : public QWidget
{
    Q_OBJECT

public:
    Uniform_Widget(int factors ,int levels, QWidget *parent = nullptr);
    ~Uniform_Widget();

    void Select_Uniform_Table(int factors, int levels);

    void Set_Result_Widget();

    void Clear_table(QVector<QTextTable*> table);

    void Draw_Regression_chart(QVector<double> predictive);

private:
    Experiment_Table *uniform_table;
    QTableWidget *table_widget;

    Uniform_Result *result_widget;

};

#endif // UNIFORM_WIDGET_H
