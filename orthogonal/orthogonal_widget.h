#ifndef ORTHOGONAL_WIDGET_H
#define ORTHOGONAL_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>

class Orthogonal_Widget : public QWidget
{
    Q_OBJECT

public:
    Orthogonal_Widget(int factors ,int levels, QWidget *parent = nullptr);
    Orthogonal_Widget(QWidget *parent = nullptr);
    ~Orthogonal_Widget();

    void Select_Orth_Table(int factors ,int levels);

    void Set_Result_Widget();

private slots:
    void TableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void TableWidget_itemChanged(QTableWidgetItem *item);

private:
    QString old_text;

    QTableWidget *table_widget;
    QWidget *reslut_widget;

    QHBoxLayout *h_layout;
    QSplitter *h_splitter;
};

#endif // ORTHOGONAL_WIDGET_H
