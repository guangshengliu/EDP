#ifndef UNIFORM_RESULT_H
#define UNIFORM_RESULT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTextTable>
#include <QTextTableFormat>
#include <QPainter>

class Uniform_Result : public QWidget
{
    Q_OBJECT

public:
    Uniform_Result(int factors ,int levels, QWidget *parent = nullptr);
    ~Uniform_Result();
    QTextEdit *result_text;

    QVector<QTextTable*> table;
    QTextTable* Three_Lines_Table(int row , int column);

private:
    QHBoxLayout *layout;

};

#endif // UNIFORM_RESULT_H
