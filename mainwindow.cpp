#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add.h"
#include "dialog_find.h"
#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->action_add,SIGNAL(triggered()),this,SLOT(addDetectionSlot()));
    connect(this->ui->action_find,SIGNAL(triggered()),this,SLOT(findDetectionSlot()));
    connect(this->ui->action_exit,SIGNAL(triggered()),this,SLOT(systemExitSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDetectionSlot()
{
    AddDialog *pAddDialog = new AddDialog(this);
    pAddDialog->show();
}

void MainWindow::findDetectionSlot()
{
    FindDialog *pFindDialog = new FindDialog(this);
    pFindDialog->show();
}

void MainWindow::systemExitSlot()
{
    switch(QMessageBox::question(this,QString::fromLocal8Bit("退出确认"),QString::fromLocal8Bit("确定要退出系统吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
    {
    case QMessageBox::Ok:
        qApp->quit();
        break;
    default:
        break;
    }
}

