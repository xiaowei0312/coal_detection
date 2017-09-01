#ifndef DIALOG_ADD_H
#define DIALOG_ADD_H

#include <QDialog>
#include <QLineEdit>
#include <QSignalMapper>
#include "data_dependency.h"

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
    QSignalMapper *signalMapper;
    bool needSave;
    bool isMarked;
    QList<QLineEdit*> editList1,editList2;
    QStringList colNameList,colValList,colNameList_print;
    QList<DataDependency*> dataDepList;

private:
    bool saveData();
    bool updateData(const QString &id);
    bool printData(const QString &id);
    bool saveOrUpdateData();
    void editListInit();
    void colListInit();
    void updateNeedSave(bool flag);
    void addMarks();
    void addMark(QLineEdit *pLineEdit);
    void clearMarks();
    void clearMark(QLineEdit *pLinEdit);
    void generateData(QLineEdit *pLinEdit);
    int generateDependencies();
    void addEventFilter();

private slots:
    void btn_add_clicked();
    void btn_print_clicked();
    void btn_exit_clicked();
    void edit_text_changed(const QString &text);
    void edit_text_changed_mapper(int i);
    void printPreview(QPrinter*);
public slots:
     bool eventFilter(QObject *,QEvent *); 
};
#endif // DIALOG_ADD_H
