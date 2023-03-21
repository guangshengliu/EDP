#ifndef REGRESSION_ANALYSIS_H
#define REGRESSION_ANALYSIS_H

#include <QWidget>
#include <QTableWidget>
#include <QtAlgorithms>
#include "lib.h"

/*
 *  Update regression equation w
 */
void Parameter_update(QVector<QVector<double>> x,QVector<double> y,QVector<double> &w,double alpha)
{
    int n = w.size();
    int m = x.size();
    QVector<double> new_w;
    for (int var = 0; var < n; ++var) {
        double sum2 = 0.0;
        for (int i = 0; i < m; ++i) {
            double sum1 = 0.0;
            for (int j = 0; j < n; ++j) {
                sum1 += w[j] * x[i][j];
            }
            sum2 += (sum1 - y[i]) * x[i][var];
        }
        new_w.append(w[var] - alpha / m * sum2);
    }
    // Replace
    for (int var = 0; var < n; ++var)
        w[var] = new_w[var];
}


double Calculation_error(QVector<QVector<double>> x,QVector<double> y,QVector<double> w)
{
    int n = w.size();
    int m = x.size();
    double sum = 0.0;
    for (int i = 0; i < m; ++i) {
        double sum1 = 0.0;
        for (int j = 0; j < n; ++j)
            sum1 += w[j] * x[i][j];
        sum = (sum1 - y[i])*(sum1 - y[i]);
    }
    return sum/(2*m);
}

// x:mx(n+1),y:1xm,w:1x(n+1)
QVector<double> Regression_analysis(QTableWidget *table_widget, double alpha, int n)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();

    QVector<QVector<double>> x(row,QVector<double>(column));
    QVector<double> y(row);
    QVector<double> w(column);
    QVector<QVector<double>> data_info(column-1,QVector<double>(2));
    QVector<QVector<double>> data(row,QVector<double>(column-1));
    data_info = Data_Normalization(data,table_widget);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column -1; ++j)
            x[i][j] = data[i][j];
        x[i][column-1] = 1.0;
    }
    for (int var = 0; var < row; ++var)
        y[var] = table_widget->item(var,column-1)->text().toDouble();
    for (int var = 0; var < column; ++var)
        w[var] = 1;

    for (int var = 0; var < n; ++var) {
        Parameter_update(x,y,w,alpha);
        //qDebug()<<"error:"<<Calculation_error(x,y,w);
    }
    for (int var = 0; var < column-1; ++var)
        w[var] = w[var] / (data_info[var][1] - data_info[var][0]);
    for (int var = 0; var < column-1; ++var)
        w[column-1] -= w[var] * data_info[var][0];
    return w;
}

//  Total Sum of Squares
double TSS(QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    double average = 0.0;
    for (int i = 0; i < row; ++i)
        average += table_widget->item(i,column-1)->text().toDouble();
    average = average / row;
    double tss = 0.0;
    for (int i = 0; i < row; ++i)
        tss += pow(table_widget->item(i,column-1)->text().toDouble() - average,2);
    return tss;
}

//  Residual Sum of Squares
double RSS(QTableWidget *table_widget, QVector<double> predictive)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    if(predictive.size() != row)
        return 0.0;
    double rss = 0.0;
    for (int i = 0; i < row; ++i)
        rss += pow(table_widget->item(i,column-1)->text().toDouble() - predictive[i],2);
    return rss;
}

//  Explained Sum of Squares
double ESS(QTableWidget *table_widget, QVector<double> predictive)
{
    double rss = RSS(table_widget,predictive);
    double tss = TSS(table_widget);
    return tss - rss;
}

//  R-Squares = (TSS-RSS)/TSS
double R_Squares(QTableWidget *table_widget, QVector<double> predictive)
{
    double rss = RSS(table_widget,predictive);
    double tss = TSS(table_widget);
    return (tss - rss) / tss;
}

//  Adjust-R-Squares
double Adjust_R_Squares(QTableWidget *table_widget, QVector<double> predictive)
{
    double rss = RSS(table_widget,predictive);
    double tss = TSS(table_widget);
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int n = row * column;
    double adjust_r_squares = 1 - (rss/(n-row-1)) / (tss/(n-1));
    return adjust_r_squares;
}

//  Adjust-R-Squares
double F_Test(QTableWidget *table_widget, QVector<double> predictive)
{
    double rss = RSS(table_widget,predictive);
    double tss = TSS(table_widget);
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int k = column - 1;
    int n = row;
    double F_value = ((tss-rss)/k) / (rss/(n-k-1));
    return F_value;
}

#endif // REGRESSION_ANALYSIS_H
