#include "MotormeterTest.h"
#include "ui_MotormeterTest.h"

MotormeterTest::MotormeterTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotormeterTest)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint
                       | Qt::WindowTitleHint
                       | Qt::WindowMinimizeButtonHint
                       | Qt::WindowCloseButtonHint
                       );

    serialport = new QSerialPort();
    isStart = true;
    isOpen = false;

    ui->textEdit->document()->setMaximumBlockCount(1000);
    ui->textEdit_2->document()->setMaximumBlockCount(1000);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
        qDebug() << "System Location: " << info.systemLocation();

        ui->comboBox->addItem(info.portName());
    }

    connect(serialport,SIGNAL(readyRead()),this,SLOT(recSerialData()));
}

MotormeterTest::~MotormeterTest()
{
    delete serialport;
    delete ui;
}

void MotormeterTest::on_pushButton_clicked()
{
    set_serial();
    if(ui->pushButton->text()=="打开串口"){
        bool com=serialport->open(QIODevice::ReadWrite);
        if(com){
            isOpen = true;
            //set_serial();
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_5->setEnabled(true);
            ui->pushButton_6->setEnabled(true);
            ui->pushButton_7->setEnabled(true);
            ui->pushButton_10->setEnabled(true);
            ui->pushButton_11->setEnabled(true);
            ui->pushButton_12->setEnabled(true);
            ui->pushButton->setText("关闭串口");
        }else{
            qDebug() << "open serial failed!";
        }
    }else{
        if(serialport->isOpen())
         {
            serialport->close();
            isOpen = false;
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->pushButton_7->setEnabled(false);
            ui->pushButton_10->setEnabled(false);
            ui->pushButton->setText("打开串口");
        }
    }
}

void MotormeterTest::on_pushButton_7_clicked()
{
    //帧头
    cmd.clear();
    QString str = ui->lineEdit->text();
    QString str1 = str.trimmed();
    bool ok;
    int num = 0;
    if(!str1.isEmpty()){
        QStringList strList = str1.split(" ");
        foreach (QString s_str, strList) {
            num = s_str.toInt(&ok,16);
            if(ok){
                cmd.append(num);
            }
        }
    }

    //命令
    QString str2 = ui->lineEdit_2->text();
    QString str3 = str2.trimmed();
    if(!str3.isEmpty()){
        num = str3.toInt(&ok,16);
        if(ok){
            num = num&0xFF;
            cmd.append(num);
        }
    }

    //长度
    QString str4 = ui->lineEdit_3->text();
    QString str5 = str4.trimmed();
    if(!str5.isEmpty()){
        QStringList strList1 = str5.split(" ");
        foreach (QString s_str, strList1) {
            num = s_str.toInt(&ok,16);
            if(ok){
                cmd.append(num);
            }
        }
    }

    //数据
    QString str6 = ui->textEdit->toPlainText();
    QString str7 = str6.trimmed();
    if(!str7.isEmpty()){
        QStringList strList2 = str7.split(" ");

        foreach (QString s_str, strList2) {
            int num = s_str.toInt(&ok,16);
            if(ok){
                cmd.append(num);
                //qDebug() << "lineedit2" << s_tr1;
            }
        }
    }

    //检验码
    QString str8 = ui->lineEdit_4->text();
    QString str9 = str8.trimmed();
    if(!str9.isEmpty()){
        num = str9.toInt(&ok,16);
        if(ok){
            num = num&0xFF;
            cmd.append(num);
        }
    }

    qDebug() << "cmd" << cmd;

    QByteArray ba;
    ba.resize(cmd.length());
    int tmp = 0;
    foreach (int s_num, cmd) {
        ba[tmp] = s_num&0xFF;
        tmp++;
    }

    qDebug() << ba.toHex();
    serialport->write(ba);

//    QString str = ui->textEdit->toPlainText();
//    qDebug() << str;
//    str.remove(QChar(' '), Qt::CaseInsensitive);
//    qDebug() << str;
//    QByteArray macAddress = QByteArray::fromHex(str);
//    serialport->write(macAddress);
}

void MotormeterTest::on_pushButton_8_clicked()
{
    ui->textEdit_2->clear();
}

void MotormeterTest::on_pushButton_9_clicked()
{
    ui->textEdit->clear();
    //ui->lineEdit_3->setText("00 00");
}

void MotormeterTest::on_pushButton_10_clicked()
{
    if(isStart){
        ui->pushButton_10->setText("启动");
    }else{
         ui->pushButton_10->setText("暂停");
    }
    isStart = !isStart;
}

void MotormeterTest::set_serial()
{
    //设置串口号
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.portName()==ui->comboBox->currentText())
        {
            serialport->setPortName(info.systemLocation());
        }
    }

    //设置波特率
    serialport->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);

    //设置数据位
    serialport->setDataBits(QSerialPort::Data8);

    //设置校验位
    serialport->setParity(QSerialPort::NoParity);

    //设置停止位
    serialport->setStopBits(QSerialPort::OneStop);
}

void MotormeterTest::recSerialData()
{
    QByteArray ba;
    ba = serialport->readAll();
    QString str = ba.toHex(' ');
    //QString str1 = str.toUpper();

    if(isStart)
        ui->textEdit_2->append(str);
}

void MotormeterTest::on_lineEdit_2_textChanged(const QString &arg1)
{
    //命令
    cmd.clear();
    QString str = ui->lineEdit_2->text();
    bool ok;
    int num = str.toInt(&ok,16);
    if(ok){
        num = num&0xFF;
        cmd.append(num);
        qDebug() << num << "asd";
    }

    //数据长度
    QString str1 = ui->lineEdit_3->text();
    QStringList strList = str1.split(" ");
    foreach (QString s_tr, strList) {
        int num1 = s_tr.toInt(&ok,16);
        if(ok){
            cmd.append(num1);
        }
    }

    //数据
    QString str3 = ui->textEdit->toPlainText();
    QString str4 = str3.trimmed();
    if(!str4.isEmpty()){
        QStringList strList2 = str4.split(" ");

        foreach (QString s_tr1, strList2) {
            int num2 = s_tr1.toInt(&ok,16);
            if(ok){
                cmd.append(num2);
                //qDebug() << "lineedit2" << s_tr1;
            }
        }
    }

    qDebug() << "cmd" << cmd;

    //校验码
    int checknum = 0;
    foreach (int tmp, cmd) {
        checknum = checknum + tmp;
    }
    checknum = checknum^0xFF;

    qDebug() << "checknum = " << checknum;

    checknum = checknum&0xFF;

    QString str5 =  QString("%1").arg(checknum,2,16,QLatin1Char('0'));

    ui->lineEdit_4->setText(str5);
}

void MotormeterTest::on_textEdit_textChanged()
{
    //命令
    cmd.clear();
    QString str = ui->lineEdit_2->text();
    bool ok;
    int num = str.toInt(&ok,16);
    if(ok){
        num = num&0xFF;
        cmd.append(num);
    }

    //数据长度
    QString str1 = ui->textEdit->toPlainText();
    QString str2 = str1.trimmed();
    if(!str2.isEmpty()){
        QStringList strList = str2.split(" ");

        int num1 = strList.length();
        int num2 = (num1&0xFF00) >> 8;
        int num3 = (num1&0xFF);
        cmd.append(num2);
        cmd.append(num3);

        QString str3 =  QString("%1").arg(num2,2,16,QLatin1Char('0'));
        QString str4 =  QString("%1").arg(num3,2,16,QLatin1Char('0'));

        QString str5 = str3+" "+str4;
        ui->lineEdit_3->setText(str5);
    }else{
        ui->lineEdit_3->setText("00 00");
    }

    //数据
    QString str6 = ui->textEdit->toPlainText();
    if(!str6.isEmpty()){
        QStringList strList2 = str6.split(" ");

        foreach (QString s_tr1, strList2) {
            int num4 = s_tr1.toInt(&ok,16);
            if(ok){
                cmd.append(num4);
                //qDebug() << "lineedit2" << s_tr1;
            }
        }
    }

    //校验码
    unsigned char checknum = 0;
    foreach (unsigned char tmp, cmd) {
        checknum = checknum + tmp;
    }
    checknum = checknum^0xFF;
    qDebug() << "checknum = " << checknum;
    //checknum = checknum&0xFF;
    QString str5 =  QString("%1").arg(checknum,2,16,QLatin1Char('0'));
    ui->lineEdit_4->setText(str5);
}

void MotormeterTest::on_pushButton_6_pressed()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x56;
    ba[7] = 0x01;
    ba[8] = 0x23;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_6_released()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x56;
    ba[7] = 0x00;
    ba[8] = 0x24;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_4_pressed()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4A;
    ba[7] = 0x01;
    ba[8] = 0x2F;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_4_released()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4A;
    ba[7] = 0x00;
    ba[8] = 0x30;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_3_pressed()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x50;
    ba[7] = 0x01;
    ba[8] = 0x29;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_3_released()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x50;
    ba[7] = 0x00;
    ba[8] = 0x2A;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_2_pressed()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4B;
    ba[7] = 0x01;
    ba[8] = 0x2E;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_2_released()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4B;
    ba[7] = 0x00;
    ba[8] = 0x2F;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_5_pressed()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4C;
    ba[7] = 0x01;
    ba[8] = 0x2d;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_5_released()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x03;
    ba[5] = 0x40;
    ba[6] = 0x4C;
    ba[7] = 0x00;
    ba[8] = 0x2E;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_11_clicked()
{
    QByteArray ba;
    ba.resize(12);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x06;
    ba[5] = 0x60;
    ba[6] = 0x00;
    ba[7] = 0x03;
    ba[8] = 0x00;
    ba[9] = 0x00;
    ba[10] = 0x00;
    ba[11] = 0x54;
    if(isOpen){
        serialport->write(ba);
    }
}

void MotormeterTest::on_pushButton_12_clicked()
{
    QByteArray ba;
    ba.resize(12);
    ba[0] = 0xAA;
    ba[1] = 0x77;
    ba[2] = 0x42;
    ba[3] = 0x00;
    ba[4] = 0x06;
    ba[5] = 0x60;
    ba[6] = 0x00;
    ba[7] = 0x06;
    ba[8] = 0x00;
    ba[9] = 0x00;
    ba[10] = 0x00;
    ba[11] = 0x51;
    if(isOpen){
        serialport->write(ba);
    }
}
