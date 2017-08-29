#include "dialog_add.h"
#include "ui_dialog_add.h"
#include <QDebug>

AddDialog::AddDialog(QWidget *parent)
     : QDialog(parent), ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}
