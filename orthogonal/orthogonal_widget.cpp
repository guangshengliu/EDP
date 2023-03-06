#include "orthogonal_widget.h"
#include "orthogonal_table.h"
#include "../lib/lib.h"
#include "../lib/special_math.h"

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
    result_widget = new Orthogonal_Result(factors,levels);
    result_widget->hide();
    h_splitter->addWidget(table_widget);
    h_splitter->addWidget(result_widget);
    // left:right = 1:1
    h_splitter->setStretchFactor(0,1);
    h_splitter->setStretchFactor(1,1);

    h_layout->addWidget(h_splitter);
    Select_Orth_Table(factors,levels);
    Keep_TableWidget_Info(info_table,levels);

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
    //set_test(table_widget);
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

// Draw scatterSeries for range anysisc
void Orthogonal_Widget::Draw_Scatter_Series(QVector<double> result)
{
    QScatterSeries *scatterSeries = new QScatterSeries();

    for (int var = 0; var < result.size(); ++var)
        scatterSeries->append(var+1,result[var]);
    scatterSeries->setPointLabelsVisible();
    scatterSeries->setPointLabelsFormat("(Factor:@xPoint,Range:@yPoint)");
    scatterSeries->setPointLabelsClipping(false);
    scatterSeries->setPointLabelsColor(Qt::blue);
    QValueAxis *x_axis = new QValueAxis();
    QValueAxis *y_axis = new QValueAxis();
    x_axis->setRange(0,result.size()+1);
    x_axis->setTickCount(result.size()+2);
    // Max element
    auto max = std::max_element(result.begin(), result.end());
    y_axis->setRange(0,int(*max)+1);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(scatterSeries);
    chart->setTitle(QStringLiteral("Range analysis values"));
    chart->addAxis(x_axis, Qt::AlignBottom);
    chart->addAxis(y_axis, Qt::AlignLeft);
    chart->resize(400,400);
    scatterSeries->attachAxis(x_axis);
    scatterSeries->attachAxis(y_axis);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}

void Orthogonal_Widget::Clear_table(QVector<QTextTable*> table)
{
    for (int var = 0; var < table.size(); ++var)
        Remove_table_text(result_widget->table[var]);
}

void Orthogonal_Widget::Set_Result_Widget()
{
    if(!Judge_QTablewidget(table_widget))
    {
        QMessageBox::critical(this,tr("critical"),tr("Please ensure that the data has been entered completely!"));
        return ;
    }
    int factors = table_widget->columnCount() - 1;
    int levels = info_table[1];

    QVector<QVector<double>> K_range(levels,QVector<double>(factors));
    for (int i = 0; i < levels; ++i)
        for (int j = 0; j < factors; ++j)
            K_range[i][j] = 0;
    Calculate_K(K_range,table_widget,info_table);
    QVector<double> K_result;
    Range_analysis(K_result,table_widget,info_table);

    //  Must clear table befor write
    Clear_table(result_widget->table);

    for (int j = 0; j < factors; ++j) {
        for (int i = 0; i < levels; ++i){
            //  Levels
            QTextCursor cell_cursor = result_widget->table[j]->cellAt(i+1,1).firstCursorPosition();
            cell_cursor.insertText(QString::asprintf("%.2lf",K_result[factors*j+i]));
            //  Free drg
            cell_cursor = result_widget->table[j]->cellAt(i+1,2).firstCursorPosition();
            cell_cursor.insertText(QString::asprintf("%d",levels-1));
            //  k-value
            cell_cursor = result_widget->table[j]->cellAt(i+1,3).firstCursorPosition();
            cell_cursor.insertText(QString::asprintf("%.2lf",K_result[factors*j+i]/(levels-1)));
        }
        //  Range
        QTextCursor cell_cursor = result_widget->table[j]->cellAt(1,4).firstCursorPosition();
        cell_cursor.insertText(QString::asprintf("%.2lf",K_result[factors*levels+j]));
    }

    QVector<double> result;
    ANOVA(result,table_widget,info_table);
    double SS_sum = 0;
    for (int var = 0; var < factors; ++var) {
        QTextCursor cell_cursor = result_widget->table[factors]->cellAt(var+1,1).firstCursorPosition();
        cell_cursor.insertText(QString::asprintf("%.2lf",result[var]));
        SS_sum += result[var];
        cell_cursor = result_widget->table[factors]->cellAt(var+1,2).firstCursorPosition();
        cell_cursor.insertText(QString::number(levels-1));
        cell_cursor = result_widget->table[factors]->cellAt(var+1,3).firstCursorPosition();
        cell_cursor.insertText(QString::asprintf("%.2lf",result[var+factors]));
        cell_cursor = result_widget->table[factors]->cellAt(var+1,4).firstCursorPosition();
        cell_cursor.insertText(QString::asprintf("%.2lf",result[var+2*factors]));
        if(result[var+2*factors] >= get_f_value(0.01,levels-1,factors)) {
            cell_cursor = result_widget->table[factors]->cellAt(var+1,6).firstCursorPosition();
            cell_cursor.insertText("**");
        }
        else if(result[var+2*factors] >= get_f_value(0.05,levels-1,factors)) {
            cell_cursor = result_widget->table[factors]->cellAt(var+1,6).firstCursorPosition();
            cell_cursor.insertText("*");
        }
        else{
            cell_cursor = result_widget->table[factors]->cellAt(var+1,6).firstCursorPosition();
            cell_cursor.insertText("Not-significant");
        }
    }
    double f_avalue = get_f_value(0.05,levels-1,factors);
    result_widget->table[factors]->cellAt(1,5).firstCursorPosition().insertText(QString::asprintf("F0.05(%d,%d)=%.2lf",levels-1,factors,f_avalue));
    f_avalue = get_f_value(0.01,levels-1,factors);
    result_widget->table[factors]->cellAt(2,5).firstCursorPosition().insertText(QString::asprintf("F0.01(%d,%d)=%.2lf",levels-1,factors,f_avalue));
    result_widget->table[factors]->cellAt(factors+1,1).firstCursorPosition().insertText(QString::asprintf("%.2lf",SS_sum));

    result_widget->show();
}
