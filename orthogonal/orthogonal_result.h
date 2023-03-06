#ifndef ORTHOGONAL_RESULT_H
#define ORTHOGONAL_RESULT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTextTable>
#include <QTextTableFormat>
#include <QPainter>

class Orthogonal_Result : public QWidget
{
    Q_OBJECT

public:
    Orthogonal_Result(int factors ,int levels, QWidget *parent = nullptr);
    ~Orthogonal_Result();
    QTextEdit *result_text;
    QVector<QTextTable*> table;
    QTextTable* Three_Lines_Table(int row , int column);

private:
    QHBoxLayout *layout;

};

void remove_text_table(QTextTable *table);

#endif // ORTHOGONAL_RESULT_H
