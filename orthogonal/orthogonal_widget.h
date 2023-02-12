#ifndef ORTHOGONAL_WIDGET_H
#define ORTHOGONAL_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>

class Orthogonal_Widget : public QWidget
{
    Q_OBJECT

public:
    Orthogonal_Widget(int factors ,int levels, QWidget *parent = nullptr);
    Orthogonal_Widget(QWidget *parent = nullptr);
    ~Orthogonal_Widget();

    QTableWidget *table_widget;
    void Select_Orth_Table(int factors ,int levels);
};

#endif // ORTHOGONAL_WIDGET_H
