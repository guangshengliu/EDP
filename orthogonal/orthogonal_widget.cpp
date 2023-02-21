#include "orthogonal_widget.h"
#include "orthogonal_table.h"
#include "../lib/lib.h"

Orthogonal_Widget::Orthogonal_Widget(int factors ,int levels, QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    table_widget = new QTableWidget;
    table_widget->setFrameShape(QFrame::Panel);

    QHBoxLayout *h_layout = new QHBoxLayout(this);
    // Left is tablewidget,right is reslutwidget
    QSplitter *h_splitter = new QSplitter(this);
    h_splitter->setStyleSheet("QSplitter:handle{background-color:grey}");
    result_widget = new Orthogonal_Result;
    h_splitter->addWidget(table_widget);
    h_splitter->addWidget(result_widget);
    // left:right = 3:1
    h_splitter->setStretchFactor(0,3);
    h_splitter->setStretchFactor(1,1);

    h_layout->addWidget(h_splitter);
    Select_Orth_Table(factors,levels);

    Keep_TableWidget_Info(info_table,levels);
    /*for (int var = 0; var < info_table.size(); ++var) {
        qDebug()<<info_table[var]<<" ";
    }*/

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
    set_test(table_widget);
    // Limit input as number
    connect(table_widget,&QTableWidget::itemDoubleClicked,this,&Orthogonal_Widget::TableWidget_itemDoubleClicked);
    connect(table_widget,&QTableWidget::itemChanged,this,&Orthogonal_Widget::TableWidget_itemChanged);
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

// Get QTableWidget_item text
void Orthogonal_Widget::TableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    old_text = item->text();
}

void Orthogonal_Widget::TableWidget_itemChanged(QTableWidgetItem *item)
{
    // If not number or old_text,recover it
    if(!Judge_Number(item->text()))
    {
       QMessageBox::critical(this,tr("critical"),tr("Please express the factor's level in numbers!"));
       item->setText(old_text);
    }
}

// Keep template for tablewidget
void Orthogonal_Widget::Keep_TableWidget_Info(QVector<int> &info_table ,int levels)
{
    int row = table_widget->rowCount();
    int column = table_widget->columnCount();
    // 1. experment factor
    info_table.push_back(column);
    // 2. experment levels
    info_table.push_back(levels);
    // 3. table row
    info_table.push_back(row);
    // 4. table column
    info_table.push_back(column);
    // 5. factor and levels information
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            QString info = table_widget->item(i,j)->text();
            QString num = info[info.size()-1];
            info_table.push_back(num.toInt());
        }
    }
}

void Orthogonal_Widget::Draw_Line_Series(QVector<double> result)
{
    QScatterSeries *scatterSeries=new QScatterSeries();

    for (int var = 0; var < result.size(); ++var)
        scatterSeries->append(var+1,result[var]);
    scatterSeries->setPointLabelsVisible();
    scatterSeries->setPointLabelsFormat("(@xPoint,@yPoint)");
    scatterSeries->setPointLabelsClipping(false);
    scatterSeries->setPointLabelsColor(Qt::blue);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(scatterSeries);
    chart->setTitle(QStringLiteral("Range analysis values"));

    //QChartView *chartView = new QChartView(chart,result_widget);
    QChartView *chartView = new QChartView(chart);

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}

void Orthogonal_Widget::Set_Result_Widget()
{
    if(!Judge_QTablewidget(table_widget))
    {
        QMessageBox::critical(this,tr("critical"),tr("Please ensure that the data has been entered completely!"));
        return ;
    }
    QVector<double> result;
    //ANOVA(result , table_widget);
    Range_analysis(result,table_widget,info_table);
    /*for (int var = 0; var < result.size(); ++var) {
        qDebug()<<result[var]<<" ";
    }*/
    Draw_Line_Series(result);
}
