#ifndef DIALOG_ADD_H
#define DIALOG_ADD_H

#include <QDialog>

namespace Ui
{
    class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QWidget *parent = 0);
    ~AddDialog();

private:
    Ui::AddDialog *ui;
};
#endif // DIALOG_ADD_H
