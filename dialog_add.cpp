#include "dialog_add.h"
#include "ui_dialog_add.h"
#include <QDebug>
#include <QtGui>
#include <QList>

AddDialog::AddDialog(QWidget *parent)
        : QDialog(parent), ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    id = -1;    //id初始值为-1
    needSave = false;
    
    QDate today = QDate::currentDate();
    QString todayStr = QString("%1-%2-%3")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'));
    ui->lineEdit_301->setText(todayStr);
    
    this->editListInit();
    
    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(btn_add_clicked()));
    connect(ui->btn_print,SIGNAL(clicked()),this,SLOT(btn_print_clicked()));
    connect(ui->btn_exit,SIGNAL(clicked()),this,SLOT(btn_exit_clicked()));
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::editListInit()
{
    editList1   << ui->lineEdit_000 
             << ui->lineEdit_001    
             << ui->lineEdit_002
             << ui->lineEdit_010
             << ui->lineEdit_011
             << ui->lineEdit_012
             << ui->lineEdit_020
             << ui->lineEdit_021
             << ui->lineEdit_022
             << ui->lineEdit_030
             << ui->lineEdit_031
             << ui->lineEdit_032
            
             << ui->lineEdit_100
             << ui->lineEdit_101
             << ui->lineEdit_102
             << ui->lineEdit_103
            
             << ui->lineEdit_200
             << ui->lineEdit_201
             << ui->lineEdit_202
             << ui->lineEdit_203
             << ui->lineEdit_204
             << ui->lineEdit_205
            
             << ui->lineEdit_210
             << ui->lineEdit_211
             << ui->lineEdit_212
             << ui->lineEdit_213
             << ui->lineEdit_214
            
             << ui->lineEdit_220
             << ui->lineEdit_221
             << ui->lineEdit_222
             << ui->lineEdit_223
             << ui->lineEdit_224;
    
    editList2 << ui->lineEdit_300
            << ui->lineEdit_301
            << ui->lineEdit_302
            << ui->lineEdit_303
            << ui->lineEdit_304
            << ui->lineEdit_305
            << ui->lineEdit_306;
    
    QList<QLineEdit*>::iterator it;
    for(it=editList1.begin();it!=editList1.end();it++)
        connect(*it,SIGNAL(textChanged(const QString&)),this,SLOT(edit_text_changed(const QString&)));
    
    for(it=editList2.begin();it!=editList2.end();it++)
        connect(*it,SIGNAL(textChanged(const QString&)),this,SLOT(edit_text_changed(const QString&)));
}

void AddDialog::edit_text_changed(const QString &text)
{
    this->needSave = true;
    if(!this->windowTitle().endsWith("(*)"))
        this->setWindowTitle(this->windowTitle()+"(*)");
}

void AddDialog::btn_add_clicked()
{ 
    if(!this->saveData())
    {
        QMessageBox:: warning(this,QString::fromLocal8Bit("失败"),
            QString::fromLocal8Bit("数据保存失败"));
        return;
    }

    QMessageBox:: information(this,QString::fromLocal8Bit("成功"),
        QString::fromLocal8Bit("数据保存成功"));
    this->needSave = false;
    if(this->windowTitle().endsWith("(*)"))
        this->setWindowTitle(this->windowTitle().replace("(*)",""));
}

void AddDialog::btn_print_clicked()
{
    qDebug() << "print";
}

void AddDialog::btn_exit_clicked()
{
    if(this->needSave)
    {
        switch(QMessageBox::question(this,QString::fromLocal8Bit("关闭确认"),
            QString::fromLocal8Bit("数据尚未保存，确定要关闭吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
        {
        case QMessageBox::Ok:
            break;
        default:
            return;
        }
    }
    this->close();
}

bool AddDialog::saveData()
{
    QString sql = "insert into t_coal_detection values(null,";
    QList<QLineEdit*>::iterator it;
    for(it = editList1.begin();it!=editList1.end();it++)
    {
        sql += "'";
        QString tmp="",text = (*it)->text();
        if(!text.isEmpty())
            tmp = QString::number((*it)->text().toDouble(),'f',4);
        sql +=(tmp + "',");
    }
    for(it = editList2.begin();it!=editList2.end();it++)
        sql += "'" + (*it)->text() + "',";
    sql += "null,null,0";
    qDebug() << sql;
}

