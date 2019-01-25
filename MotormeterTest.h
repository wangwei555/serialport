#ifndef MOTORMETERTEST_H
#define MOTORMETERTEST_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

namespace Ui {
class MotormeterTest;
}

class MotormeterTest : public QDialog
{
    Q_OBJECT

public:
    explicit MotormeterTest(QWidget *parent = 0);
    ~MotormeterTest();

    void set_serial();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void recSerialData();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_textEdit_textChanged();

    void on_pushButton_6_pressed();

    void on_pushButton_6_released();

    void on_pushButton_4_pressed();

    void on_pushButton_4_released();

    void on_pushButton_3_pressed();

    void on_pushButton_3_released();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

    void on_pushButton_5_pressed();

    void on_pushButton_5_released();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::MotormeterTest *ui;
    QSerialPort *serialport;
    QSerialPortInfo com_info;
    QList<unsigned char> cmd;

    bool isStart;
    bool isOpen;
};

#endif // MOTORMETERTEST_H
