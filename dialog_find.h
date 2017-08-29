#ifndef DIALOG_FIND_H
#define DIALOG_FIND_H

#include <QtGui/QDialog>

namespace Ui {
    class FindDialog;
}

class FindDialog : public QDialog {
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FindDialog *ui;
private slots:    
    void btn_find_clicked();
    void btn_print_clicked();
    void btn_exit_clicked();
};

#endif // DIALOG_FIND_H
