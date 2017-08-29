#ifndef DIALOG_ADD_H
#define DIALOG_ADD_H

#include <QDialog>
#include <QLineEdit>

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
    int id;
    bool needSave;
    QList<QLineEdit*> editList1,editList2;
    bool saveData();
    void editListInit();

private slots:
    void btn_add_clicked();
    void btn_print_clicked();
    void btn_exit_clicked();
    void edit_text_changed(const QString &text);
};
#endif // DIALOG_ADD_H
