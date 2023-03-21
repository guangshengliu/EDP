#ifndef UNIFORM_TABLE_H
#define UNIFORM_TABLE_H

#include <QTableWidget>
#include <QtAlgorithms>

//  return greatest common divisor
int Greatest_Common_Divisor(int x, int y)
{
    int z = y;
    while(x % y != 0)
    {
        z = x % y;
        x = y;
        y = z;
    }
    return z;
}

//  return generate_vector base on levels
void Get_vector(QVector<int> &generate_vector, int levels)
{
    for (int var = 1; var < levels; ++var) {
        if(Greatest_Common_Divisor(var,levels) == 1)
            generate_vector.append(var);
    }
}

//  return Centered L2 deviation == CD2
double Get_Table_CD2(QVector<QVector<int>> uniform_table)
{
    int n = uniform_table.size();
    int s = uniform_table[0].size();
    QVector<QVector<double>> X(n,QVector<double>(s));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < s; ++j)
            X[i][j] = (2.0*uniform_table[i][j]-1)/(2.0*n);

    double cs1[n];
    for (int i = 0; i < n; ++i) {
        double CSP = 1.0;
        for (int j = 0; j < s; ++j)
            CSP = CSP*(2.0+abs(X[i][j]-0.5)-pow(X[i][j]-0.5,2.0));
        cs1[i] = CSP;
    }
    double CS1 = 0.0;
    for (int var = 0; var < n; ++var)
        CS1 += cs1[var];

    double cs2[n][n];
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double CSP = 1.0;
            for (int j = 0; j < s; ++j)
                CSP = CSP*(1.0+0.5*abs(X[i][j]-0.5)+0.5*abs(X[k][j]-0.5)-0.5*abs(X[i][j]-X[k][j]));
            cs2[i][k] = CSP;
        }
    }
    double CS2 = 0.0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            CS2 += cs2[i][j];
    double CS = pow(13.0/12,s)-pow(2.0,1-s)/n*CS1+1.0/pow(n,2.0)*CS2;
    double CD2 = sqrt(CS);
    return CD2;
}


void Uniform_Table(QVector<QVector<int>> &uniform_table ,int row)
{
    QVector<int> generate_vector;
    generate_vector.clear();
    Get_vector(generate_vector,row);
    int column = generate_vector.size();

    if(row < column)
        return ;

    QVector<int> uniform_row;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            if((i+1) * generate_vector[j] % row)
                uniform_row.append((i+1) * generate_vector[j] % row);
            else
                uniform_row.append(row);
        }
        uniform_table.append(uniform_row);
        uniform_row.clear();
    }
}

void Asterisk_Uniform_Table(QVector<QVector<int>> &uniform_table ,int row)
{
    QVector<int> generate_vector;
    generate_vector.clear();
    Get_vector(generate_vector,row+1);
    int column = generate_vector.size();

    if(row < column)
        return ;

    QVector<int> uniform_row;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            if((i+1) * generate_vector[j] % (row+1))
                uniform_row.append((i+1) * generate_vector[j] % (row+1));
            else
                uniform_row.append(row+1);
        }
        uniform_table.append(uniform_row);
        uniform_row.clear();
    }
}

/*
 *  iPos : Data array starts recursion position
 *  iProc : Data array starts recursion position
 *  iTol : Data array size,Combinate total number
 *  iKey : The number of taken out
 *  data : Array to be sorted
 *  des : Keep reslut
 */
void Combinate(int iPos, int iProc, int iTol, int iKey, int data[], int des[], QVector<int> &result)
{
    if(iProc > iTol) {
        return ;
    }
    if(iPos == iKey) {
        for(int i = 0;i < iKey; i++)
            result.push_back(des[i]);
        return ;
    }
    else {
        Combinate(iPos,iProc+1,iTol,iKey,data,des,result);
        des[iPos] = data[iProc];
        Combinate(iPos+1,iProc+1,iTol,iKey,data,des,result);
    }
}

// Select the best column sequence according to the value of CD2
// Return select table
QVector<QVector<int>> Select_Uniform_Column(QVector<QVector<int>> uniform_table, int factors)
{
    int row = uniform_table.size();
    int column = uniform_table[0].size();
    QVector<QVector<int>> select_table(row,QVector<int>(factors));
    // Combinate , keep result in result vector
    int data[column],des[column];
    for (int var = 0; var < column; ++var) {
        data[var] = var + 1;
        des[var] = 0;
    }
    QVector<int> combinate_result;
    Combinate(0,0,column,factors,data,des,combinate_result);
    //  Select min CD2
    QVector<double> select_CD2;
    int itvar = 0;
    while (itvar < combinate_result.size()) {
        for (int j = 0; j < factors; ++j) {
            int column_num = combinate_result[itvar] -1 ;
            for (int i = 0; i < row; ++i) {
                select_table[i][j] = uniform_table[i][column_num];
            }
            ++itvar;
        }
        double CD2 = Get_Table_CD2(select_table);
        select_CD2.push_back(CD2);
    }
    double min = 9999.99;
    int min_index = 0;
    for (int i = 0; i < select_CD2.size(); ++i) {
        if(min > select_CD2[i]){
            min = select_CD2[i];
            min_index = i;
        }
    }
    // Return
    int num = min_index  * factors;
    QVector<int> sort;
    if(num + factors -1 <= combinate_result.size()){
        for (int var = 0; var < factors; ++var)
            sort.append(combinate_result[num++]);
    }
    // Sort accroding to column_sort
    std::sort(sort.begin(),sort.end());
    for (int j = 0; j < factors; ++j) {
        for (int i = 0; i < uniform_table.size(); ++i)
            select_table[i][j] = uniform_table[i][sort[j]-1];
    }
    return select_table;
}

// Set Uniform table
void Set_Uniform_Table(QTableWidget *table_widget ,int factors ,int levels)
{
    //  It is recommended to select orthogonal experiment
    if(factors > levels)
        return ;
    table_widget->setRowCount(levels);
    table_widget->setColumnCount(factors);

    QVector<QVector<int>> uniform_table;
    // Asterisk_Uniform_Table or None_Asterisk_Uniform_Table
    Asterisk_Uniform_Table(uniform_table,levels);
    if(uniform_table[0].size() < factors)
    {
        if(levels%2 == 1)
        {
            uniform_table.clear();
            Uniform_Table(uniform_table,levels);
            if(uniform_table[0].size() < factors)
                return ;
        }
        else
            return ;
    }
    QVector<QVector<int>> table(levels,QVector<int>(factors));
    table = Select_Uniform_Column(uniform_table,factors);
    for (int i = 0; i < levels; ++i){
        for (int j = 0; j < factors; ++j){
            table_widget->setItem(i,j,new QTableWidgetItem(QString::asprintf("Level %d",table[i][j])));
            // set text in the center
            table_widget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

#endif // UNIFORM_TABLE_H
