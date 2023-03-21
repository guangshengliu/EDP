#ifndef LIB_H
#define LIB_H

#include <QScreen>
#include <QGuiApplication>
#include <QTableWidget>
#include <QRegularExpression>
#include <QMessageBox>
#include <QVector>
#include <QTextTable>


static int Get_QScreen_Widget()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.width();
}

static int Get_QScreen_Height()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    return screenRect.height();
}

static void Get_info_table(QVector<QVector<int>> &info, QVector<int> info_table)
{
    int row = info_table[2];
    int column = info_table[3];
    int itor = 4;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            info[i][j] = info_table[itor];
            ++itor;
        }
    }
}

// Make table_widget data standardization
static QVector<QVector<double>> Data_Standardization(QVector<QVector<double>> &data, QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    //  Keep average and variance
    QVector<QVector<double>> data_info(column-1,QVector<double>(2));
    double average_column[column-1];
    double variance_column[column-1];
    // Initialization
    for (int j = 0; j < column-1; ++j)
    {
        average_column[j] = 0;
        variance_column[j] = 0;
    }
    // Get Max number in a column
    for (int j = 0; j < column-1; ++j) {
        for (int i = 0; i < row; ++i) {
            // Get Data form table_widget
            data[i][j] = table_widget->item(i,j)->text().toDouble();
            average_column[j] += data[i][j];
        }
        average_column[j] = average_column[j]/row;
    }
    // Variance
    for (int j = 0; j < column-1; ++j) {
        for (int i = 0; i < row; ++i) {
            variance_column[j] += pow(data[i][j] - average_column[j],2);
        }
        variance_column[j] = sqrt(variance_column[j]/row);
    }
    //  Standardization
    for (int j = 0; j < column-1; ++j)
        for (int i = 0; i < row; ++i)
            data[i][j] = (data[i][j] - average_column[j])/variance_column[j];
    for (int var = 0; var < column-1; ++var) {
        data_info[var][0] = average_column[var];
        data_info[var][1] = variance_column[var];
    }
    return data_info;
}

// Make table_widget data normalization
static QVector<QVector<double>> Data_Normalization(QVector<QVector<double>> &data, QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    //  Keep Linear number
    QVector<QVector<double>> data_info(column-1,QVector<double>(2));
    // Get Min number in every column
    for (int j = 0; j < column-1; ++j) {
        double min = INT32_MAX;
        double max = -1;
        for (int i = 0; i < row; ++i) {
            // Get Data form table_widget
            data[i][j] = table_widget->item(i,j)->text().toDouble();
            if(min > data[i][j])
                min = data[i][j];
            if(max < data[i][j])
                max = data[i][j];
        }
        data_info[j][0] = min;
        data_info[j][1] = max;
    }
    // Normalization
    for (int j = 0; j < column-1; ++j) {
        for (int i = 0; i < row; ++i) {
            data[i][j] = (data[i][j] - data_info[j][0])/(data_info[j][1] - data_info[j][0]);
        }
    }
    return data_info;
}

// Calculate same factor and different level sum
static void Calculate_K(QVector<QVector<double>> &K_range, QTableWidget *table_widget, QVector<int> info_table)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int levels = info_table[1];

    QVector<QVector<int>> tabel_template(info_table[2],QVector<int>(info_table[3]));
    Get_info_table(tabel_template,info_table);

    for (int j = 0; j < column-1; ++j) {
        int num =1;
        while (num <= levels) {
            for (int i = 0; i < row; ++i) {
                if(tabel_template[i][j] == num)
                    K_range[num-1][j] += table_widget->item(i,column-1)->text().toDouble();
            }
            ++num;
        }
    }
}

// Range analysis
static void Range_analysis(QVector<double> &result, QTableWidget *table_widget, QVector<int> info_table)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int levels = info_table[1];

    QVector<QVector<double>> K_range(levels,QVector<double>(column-1));
    for (int i = 0; i < levels; ++i)
        for (int j = 0; j < column-1; ++j)
            K_range[i][j] = 0;

    Calculate_K(K_range,table_widget,info_table);
    //  return K-Value
    for (int i = 0; i < levels; ++i)
        for (int j = 0; j < column-1; ++j)
            result.push_back(K_range[i][j]);

    //  return range
    QVector<double> sort;
    for (int j = 0; j < column-1; ++j) {
        for (int i = 0; i < levels; ++i)
            sort.push_back(K_range[i][j]/levels);
        auto max = std::max_element(sort.begin(), sort.end());
        auto min = std::min_element(sort.begin(), sort.end());
        double range = *max - *min;
        // Keep 2 decimal places
        QString range_str2 = QString::number(range,'f',2);
        range = range_str2.toDouble();
        result.push_back(range);
        sort.clear();
    }
}

/*  Analysis of variance
 *  sum of squares,SS
 *  sum of squares for total,SST
 *  sum of squares for factorA,SSA
 *  sum of squares for error,SSE
 *
*/
static void ANOVA(QVector<double> &result, QTableWidget *table_widget, QVector<int> info_table)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int factors = column-1;
    int levels = info_table[1];

    int df_T = row*column - 1;

    QVector<QVector<double>> K_range(levels,QVector<double>(factors));
    for (int i = 0; i < levels; ++i)
        for (int j = 0; j < factors; ++j)
            K_range[i][j] = 0.0;

    Calculate_K(K_range,table_widget,info_table);

    double T = 0.0;
    double QT = 0.0;
    for (int var = 0; var < row; ++var) {
        T += table_widget->item(var,factors)->text().toDouble();
        QT += qPow(table_widget->item(var,factors)->text().toDouble(),2);
    }
    double CT = qPow(T,2)/row;
    double SSA[factors];
    // SSA and Average of SSA
    double AV_SSA[factors];
    for (int j = 0; j < factors; ++j){
        double SSum = 0.0;
        for (int i = 0; i < levels; ++i){
            SSum += qPow(K_range[i][j],2);
        }
        SSA[j] = SSum/levels - CT;
        result.push_back(SSA[j]);
    }
    // Average of SSA
    for (int j = 0; j < factors; ++j) {
        AV_SSA[j] = SSA[j]/(levels - 1);
        result.push_back(AV_SSA[j]);
    }
    // F-Value
    for (int j = 0; j < factors; ++j){
        if(AV_SSA[j] >= AV_SSA[factors-1])
        {
            double cal = AV_SSA[j]/AV_SSA[factors-1];
            result.push_back(cal);
        }
        else {
            result.push_back(0);
        }
    }
}

//  Judge whether the string is numeric
static bool Judge_Number(QString str)
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
static bool Judge_QTablewidget(QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int sum = 0;
    for (int i = 0; i < row; ++i) {
        QString str = table_widget->item(i,column-1)->text();
        if(!str.toDouble() || str.toDouble()<=0)
            return false;
    }
    return true;
}

//  Judge whether the all QTablewidget is numeric
static bool Judge_All_QTablewidget(QTableWidget *table_widget)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    int sum = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            QString str = table_widget->item(i,j)->text();
            if(!str.toDouble() || str.toDouble()<=0)
                return false;
        }
    }
    return true;
}

static bool set_test_orth(QTableWidget *table_widget)
{
    table_widget->setRowCount(9);
    table_widget->setColumnCount(5);
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
        for (int j = 0; j < 4; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::number(result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    double num[9] = {11.43,15.49,19.94,19.84,21.3,18.95,21.03,19.27,21.34};
    for (int var = 0; var < 9 ; ++var)
    {
        table_widget->setItem(var,4,new QTableWidgetItem(QString::number(num[var])));
        table_widget->item(var,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    return true;
}

static bool set_test_unifrom(QTableWidget *table_widget)
{
    table_widget->setRowCount(9);
    table_widget->setColumnCount(3);
    double result[9][3] = {136.5,200,5.8,
                      137,240,6.3,
                      137.5,190,4.9,
                      138,230,5.4,
                      138.5,180,4,
                      139,220,4.5,
                      139.5,170,3,
                      140,210,3.6,
                      140.5,250,4.1};
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 3; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::number(result[i][j])));
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    return true;
}

static bool Remove_table_text(QTextTable *table)
{
    int row = table->rows();
    int column = table->columns();
    for (int i = 1; i < row; ++i) {
        for (int j = 1; j < column; ++j) {
            QTextCursor cell_beg = table->cellAt(i,j).firstCursorPosition();
            QTextCursor cell_end = table->cellAt(i,j).lastCursorPosition();
            while (cell_beg != cell_end) {
                cell_beg.deleteChar();
            }
        }
    }
    return true;
}

#endif // LIB_H
