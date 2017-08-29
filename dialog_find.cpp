#include "dialog_find.h"
#include "ui_dialog_find.h"
#include <QtGui>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    connect(ui->btn_find,SIGNAL(clicked()),this,SLOT(btn_find_clicked()));
    connect(ui->btn_print,SIGNAL(clicked()),this,SLOT(btn_print_clicked()));
    connect(ui->btn_exit,SIGNAL(clicked()),this,SLOT(btn_exit_clicked()));
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void FindDialog::btn_find_clicked()
{
    qDebug() << "find";
}

void FindDialog::btn_print_clicked()
{
    qDebug() << "print";
}


void FindDialog::btn_exit_clicked()
{
   switch(QMessageBox::question(this,QString::fromLocal8Bit("关闭确认"),QString::fromLocal8Bit("确定要关闭吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
   {
    case QMessageBox::Ok:
        this->close();
        break;
    default:
        break;
    }
}
