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
    void closeEvent ( QCloseEvent * event );

private:
    Ui::AddDialog *ui;
    bool needSave;
    bool isMarked;
    QList<QLineEdit*> editList1,editList2;
    QStringList colNameList,colValList;
    bool saveData();
    bool updateData(const QString &id);
    bool printData(const QString &id);
    bool saveOrUpdateData();
    void editListInit();
    void colListInit();
    void updateNeedSave(bool flag);
    void addMarks();
    void clearMarks();

private slots:
    void btn_add_clicked();
    void btn_print_clicked();
    void btn_exit_clicked();
    void edit_text_changed(const QString &text);
};
#endif // DIALOG_ADD_H
