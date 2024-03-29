#ifndef UNIFORM_DIALOG_H
#define UNIFORM_DIALOG_H

#include <QDialog>
#include <Qpushbutton>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

class Uniform_Dialog : public QDialog
{
    Q_OBJECT

public:
    Uniform_Dialog(QWidget *parent = nullptr);
    ~Uniform_Dialog();
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

#endif // UNIFORM_DIALOG_H
