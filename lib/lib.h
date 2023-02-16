#ifndef LIB_H
#define LIB_H

#include <QScreen>
#include <QGuiApplication>
#include <QTableWidget>
#include <QRegularExpression>
#include <QMessageBox>


int Get_QScreen_Widget()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.width();
}

int Get_QScreen_Height()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.height();
}

//  Analysis of variance
QList<double> ANOVA(QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int sum = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            // Get Data form table_widget
            sum += table_widget->item(i,j)->text().toDouble();
        }
    }

    QList<double> result;
    result.append(sum);

    return result;
}

//  Judge whether the string is numeric
bool Judge_Number(QString str)
{
    // Regular matching expression
    QString Pattern("(-?[1-9][0-9]+)|(-?[0-9])|(-?[1-9]\\d+\\.\\d+)|(-?[0-9]\\.\\d+)");
    QRegularExpression  reg_exp(Pattern);
    // Match yes or no?
    QRegularExpressionMatch match = reg_exp.match(str);
    if(!match.hasMatch())
        return false;
    return true;
}

//  Judge whether the QTablewidget is numeric
bool Judge_QTablewidget(QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int sum = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            // Get Data form table_widget
            QString str = table_widget->item(i,j)->text();
            if(!str.toDouble())
                return false;
        }
    }
    return true;
}

#endif // LIB_H
