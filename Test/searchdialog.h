#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QtGui>

class SearchDialog:public QDialog
{
private:
   QLineEdit *m_pSearchLineEdit;
public:
    SearchDialog(QWidget *parent=0);
private slots:
    void search();
};

#endif // SEARCHDIALOG_H
