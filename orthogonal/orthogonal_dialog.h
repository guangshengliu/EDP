#ifndef ORTHOGONAL_DIALOG_H
#define ORTHOGONAL_DIALOG_H

#include <QDialog>
#include <Qpushbutton>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

class Orthogonal_Dialog : public QDialog
{
    Q_OBJECT

public:
    Orthogonal_Dialog(QWidget *parent = nullptr);
    ~Orthogonal_Dialog();
    QPushButton *confirm;

protected:
    QPushButton *cancel;
    QComboBox   *factors;
    QComboBox   *levels;
    QGridLayout *gridlayout;

signals:
    void Send_Messages(int factor ,int level);

private slots:
    void confirm_clicked();
};

#endif // ORTHOGONAL_DIALOG_H
