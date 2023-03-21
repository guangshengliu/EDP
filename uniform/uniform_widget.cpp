#include "uniform_widget.h"
#include "uniform_table.h"
#include "uniform_result.h"
#include "../lib/lib.h"
#include "../lib/special_math.h"
#include "../lib/regression_analysis.h"
#include "../lib/matrix.h"

Uniform_Widget::Uniform_Widget(int factors ,int levels, QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    uniform_table = new Experiment_Table;
    table_widget = uniform_table->experiment_table;
    QHBoxLayout *h_layout = new QHBoxLayout(this);
    // Left is tablewidget,right is reslutwidget
    QSplitter *h_splitter = new QSplitter(this);
    h_splitter->setStyleSheet("QSplitter:handle{background-color:grey}");
    result_widget = new Uniform_Result(factors,levels);
    result_widget->hide();
    h_splitter->addWidget(table_widget);
    h_splitter->addWidget(result_widget);
    // left:right = 1:1
    h_splitter->setStretchFactor(0,1);
    h_splitter->setStretchFactor(1,1);

    h_layout->addWidget(h_splitter);
    Select_Uniform_Table(factors,levels);

    QList<QString> title;
    for (int var = 1; var < factors + 1; ++var)
        title.append(QString::asprintf("Factor %d",var));
    table_widget->setHorizontalHeaderLabels(title);
    // Insert result column
    table_widget->insertColumn(factors);
    table_widget->setHorizontalHeaderItem(factors,new QTableWidgetItem("Result"));
    for (int var = 0; var < table_widget->rowCount() ; ++var)
    {
        table_widget->setItem(var,factors,new QTableWidgetItem(QString::asprintf("Result %d",var+1)));
        table_widget->item(var,factors)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    //set_test_unifrom(table_widget);
}

Uniform_Widget::~Uniform_Widget()
{
}

void Uniform_Widget::Select_Uniform_Table(int factors, int levels)
{
    Set_Uniform_Table(table_widget,factors,levels);
}

void Uniform_Widget::Clear_table(QVector<QTextTable*> table)
{
    for (int var = 0; var < table.size(); ++var)
        Remove_table_text(result_widget->table[var]);
}

void Uniform_Widget::Set_Result_Widget()
{
    if(!Judge_All_QTablewidget(table_widget))
    {
        QMessageBox::critical(this,tr("critical"),tr("Please ensure that the data has been entered completely!"));
        return ;
    }
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();

    QVector<double> w(column);
    w = Regression_analysis(table_widget,0.1,1500);
    Matrix<double> x(row,column);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j)
            x.setValue(i,j,table_widget->item(i,j)->text().toDouble());
        x.setValue(i,column-1,1);
    }
    Matrix<double> beta(column,1);
    for (int var = 0; var < column; ++var)
        beta.setValue(var,0,w[var]);
    Matrix<double> y(row,1);
    y = x * beta;
    QVector<double> predictive;
    for (int var = 0; var < row; ++var) {
        double y_data = y.getValue(var,0);
        predictive.append(y_data);
    }

    //  Must clear table befor write
    Clear_table(result_widget->table);

    // Set sumarry table
    QTextCursor cell_cursor = result_widget->table[0]->cellAt(1,1).firstCursorPosition();
    double R = R_Squares(table_widget,predictive);
    cell_cursor.insertText(QString::asprintf("%.2lf",R));
    cell_cursor = result_widget->table[0]->cellAt(1,2).firstCursorPosition();
    double Adjust_R = Adjust_R_Squares(table_widget,predictive);
    cell_cursor.insertText(QString::asprintf("%.2lf",Adjust_R));
    cell_cursor = result_widget->table[0]->cellAt(1,3).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%.2lf",RSS(table_widget,predictive)/(row-column-1)));
    // Set anvoa table
    cell_cursor = result_widget->table[1]->cellAt(1,1).firstCursorPosition();
    double ess = ESS(table_widget,predictive);
    cell_cursor.insertText(QString::asprintf("%.2lf",ess));
    cell_cursor = result_widget->table[1]->cellAt(2,1).firstCursorPosition();
    double rss = RSS(table_widget,predictive);
    cell_cursor.insertText(QString::asprintf("%.2lf",rss));
    cell_cursor = result_widget->table[1]->cellAt(3,1).firstCursorPosition();
    double tss = TSS(table_widget);
    cell_cursor.insertText(QString::asprintf("%.2lf",tss));
    // Free degree
    int k = column-1;
    int n = row;
    cell_cursor = result_widget->table[1]->cellAt(1,2).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%d",k));
    cell_cursor = result_widget->table[1]->cellAt(2,2).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%d",n-k-1));
    cell_cursor = result_widget->table[1]->cellAt(3,2).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%d",n-1));
    // Mean square
    cell_cursor = result_widget->table[1]->cellAt(1,3).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%.2lf",ess/k));
    cell_cursor = result_widget->table[1]->cellAt(2,3).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%.2lf",rss/(n-k-1)));
    // F-value
    double F_value = F_Test(table_widget,predictive);
    cell_cursor = result_widget->table[1]->cellAt(1,4).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("%.2lf",F_value));
    // F-value
    double f_value_1 = get_f_value(0.1,k,n-k-1);
    double f_value_2 = get_f_value(0.05,k,n-k-1);
    cell_cursor = result_widget->table[1]->cellAt(1,5).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("F0.1(%d,%d)=%.2lf",k,n-k-1,f_value_1));
    cell_cursor = result_widget->table[1]->cellAt(2,5).firstCursorPosition();
    cell_cursor.insertText(QString::asprintf("F0.05(%d,%d)=%.2lf",k,n-k-1,f_value_2));
    // Significant levels
    if(F_value >= f_value_2) {
        cell_cursor = result_widget->table[1]->cellAt(1,6).firstCursorPosition();
        cell_cursor.insertText("**");
    }
    else if(F_value >= f_value_1) {
        cell_cursor = result_widget->table[1]->cellAt(1,6).firstCursorPosition();
        cell_cursor.insertText("*");
    }
    else{
        cell_cursor = result_widget->table[1]->cellAt(1,6).firstCursorPosition();
        cell_cursor.insertText("Not-significant");
    }
    result_widget->show();
}

// Draw scatterSeries for range anysisc
void Uniform_Widget::Draw_Regression_chart(QVector<double> predictive)
{
    QScatterSeries *scatter_predictive = new QScatterSeries();

    for (int var = 0; var < predictive.size(); ++var)
        scatter_predictive->append(var+1,predictive[var]);
    scatter_predictive->setPointLabelsVisible();
    scatter_predictive->setPointLabelsFormat("Predictive value:@yPoint");
    scatter_predictive->setPointLabelsClipping(false);
    scatter_predictive->setPointLabelsColor(Qt::blue);
    QValueAxis *x_axis = new QValueAxis();
    QValueAxis *y_axis = new QValueAxis();

    x_axis->setRange(0,predictive.size()+1);
    x_axis->setTickCount(predictive.size()+2);
    // Max element
    auto max = std::max_element(predictive.begin(), predictive.end());
    y_axis->setRange(0,int(*max)+1);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(scatter_predictive);
    chart->setTitle(QStringLiteral("Predicted vs. Actual value"));
    chart->addAxis(x_axis, Qt::AlignBottom);
    chart->addAxis(y_axis, Qt::AlignLeft);
    chart->resize(600,600);
    scatter_predictive->attachAxis(x_axis);
    scatter_predictive->attachAxis(y_axis);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}
