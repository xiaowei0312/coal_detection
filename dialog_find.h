#ifndef DIALOG_FIND_H
#define DIALOG_FIND_H

#include <QtGui/QDialog>
#include <QLineEdit>

namespace Ui {
    class FindDialog;
}

class FindDialog : public QDialog {
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog();
    void closeEvent ( QCloseEvent * event );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FindDialog *ui;
    
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
    void showDataById(const QString &id);
    
    void addMarks();
    void clearMarks();
    
private slots:    
    void btn_find_clicked();
    void btn_print_clicked();
    void btn_exit_clicked();
    void btn_update_clicked();
    
    void edit_text_changed(const QString &text);
    void searchkey_text_changed(const QString &text);
};

#endif // DIALOG_FIND_H
