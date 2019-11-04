#include "dialog.h"
#include "./ui_dialog.h"
#include <stdio.h>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ClientSlot(QString info)
{
    QString flag = info.mid(0, 2);
    if(QString("ws") == flag)
    {
        ui->WorkstationInfo->setText(info.mid(3));
    }
    else if(QString("fp") == flag)
    {
        ui->ParsingInfo->setText(info.mid(3));
    }
    else
    {
        QByteArray sr=info.toLocal8Bit();
        char ch[1024];
        strcpy(ch,sr.data());
        printf("cann't parsing server info:\t%s\n", ch);
    }
}
