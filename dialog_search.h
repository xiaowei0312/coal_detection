#ifndef DIALOG_SEARCH_H
#define DIALOG_SEARCH_H

#include <QtGui/QDialog>
#include <QList>
#include <QString>
#include <QTreeWidgetItem>

namespace Ui {
    class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT
public:
    SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchDialog *ui;
    QString m_szSearchKey;
    int m_nCurrentPage;
    int m_nSearchFlag;
    int m_nMaxPage;
    int m_nCountPerPage;
    QList<QStringList> m_oDataList; 
    void setBtnsStatus();
    int getMaxPage();
    void searchData();
    void showData(QList<QStringList> values);
    

private slots:
    void editTextChanged(const QString &);
    void searchBtnClicked();
    void firstPageClicked();
    void lastPageClicked();
    void previousPageClicked();
    void nextPageClicked();
    void itemDbClicked(QTreeWidgetItem *item,int column);
};

#endif // DIALOG_SEARCH_H
