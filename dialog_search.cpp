#include "dialog_search.h"
#include "ui_dialog_search.h"
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include "dbutil.h"
#include "dialog_find.h"

SearchDialog::SearchDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    ui->m_pTreeWidget->clear();
    
    ui->m_pSearchLineEdit->setValidator(new QRegExpValidator(
                        QRegExp("^\\d{1,11}$"),this));
    
    m_nSearchFlag = 0;
    m_nCurrentPage = 1;
    m_nMaxPage = 1;
    m_nCountPerPage = 3;
    setBtnsStatus();
    
    connect(ui->m_pSearchLineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(editTextChanged(const QString &)));
    connect(ui->m_pSearchBtn,SIGNAL(clicked()),this,SLOT(searchBtnClicked()));
    connect(ui->m_pFirstPageBtn,SIGNAL(clicked()),this,SLOT(firstPageClicked()));
    connect(ui->m_pLastPageBtn,SIGNAL(clicked()),this,SLOT(lastPageClicked()));
    connect(ui->m_pPreviousPageBtn,SIGNAL(clicked()),this,SLOT(previousPageClicked()));
    connect(ui->m_pNextPageBtn,SIGNAL(clicked()),this,SLOT(nextPageClicked()));
    connect(ui->m_pTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(itemDbClicked(QTreeWidgetItem *,int)));
}

SearchDialog::~SearchDialog()
{
    delete ui;
}


void SearchDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SearchDialog::setBtnsStatus()
{
    ui->m_pFirstPageBtn->setEnabled(true);
    ui->m_pLastPageBtn->setEnabled(true);
    ui->m_pNextPageBtn->setEnabled(true);
    ui->m_pPreviousPageBtn->setEnabled(true);
    ui->m_pSearchBtn->setEnabled(true);
    
    if(m_nSearchFlag == 0)    //未开始
    {
        ui->m_pFirstPageBtn->setEnabled(false);
        ui->m_pLastPageBtn->setEnabled(false);
        ui->m_pNextPageBtn->setEnabled(false);
        ui->m_pPreviousPageBtn->setEnabled(false);
        ui->m_pSearchBtn->setEnabled(false);
    }
    if(m_nCurrentPage==1)
    {
        ui->m_pPreviousPageBtn->setEnabled(false);
        ui->m_pFirstPageBtn->setEnabled(false);
    }
    if(m_nCurrentPage==m_nMaxPage)
    {
        ui->m_pNextPageBtn->setEnabled(false);
        ui->m_pLastPageBtn->setEnabled(false);
    }
}

void SearchDialog::editTextChanged(const QString &text)
{
    if(!text.trimmed().isEmpty())
        ui->m_pSearchBtn->setEnabled(true);
    else
        ui->m_pSearchBtn->setEnabled(false);
}

void SearchDialog::showData(QList<QStringList> values)
{
    ui->m_pTreeWidget->clear();
    for(int i=0;i<values.size();i++)
        ui->m_pTreeWidget->addTopLevelItem(new QTreeWidgetItem(values.value(i)));
    ui->m_pPageLabel->setText(QString::fromLocal8Bit("第%1页 / 共%2页").arg(m_nCurrentPage).arg(m_nMaxPage));
}

void SearchDialog::searchData()
{
    QStringList names;
    QList<QStringList> values;
    QString expressions("(id = %1 or data_303 = %2) and is_deleted = 0");
    expressions += " order by add_time desc";
    expressions += " limit %3,%4";
    
    names << "id" << "data_303" << "data_302" << "data_301" << "add_time";
    expressions = expressions.arg(m_szSearchKey).arg(m_szSearchKey)
            .arg((m_nCurrentPage-1) * m_nCountPerPage)
            .arg(m_nCountPerPage);
    
    if(!DBUtil::getInstance()->select("t_coal_detection",names,expressions,values))
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("查找失败，请检查搜索字符串格式是否正确"));
        return;
    }
    if(values.size()==0)
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("没有找到对应数据"));
        return;
    }
    
    showData(values);
    setBtnsStatus();
}

int SearchDialog::getMaxPage()
{
    QStringList names;
    QList<QStringList> values;
    QString expressions("(id = %1 or data_303 = %2) and is_deleted = 0");
    
    names << "count(*)";
    expressions = expressions.arg(m_szSearchKey).arg(m_szSearchKey);
    
    if(!DBUtil::getInstance()->select("t_coal_detection",names,expressions,values))
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
                               QString::fromLocal8Bit("查找失败，请检查搜索字符串格式是否正确"));
        return 0;
    }
    int totalCount = ((QString)values.value(0).value(0)).toInt();
    return (totalCount - 1) / m_nCountPerPage + 1;
}

void SearchDialog::searchBtnClicked()
{
    QString key = ui->m_pSearchLineEdit->text();
    if(key.trimmed().isEmpty())
        return;
    
    this->m_nSearchFlag = 1;
    this->m_nCurrentPage = 1;
    this->m_szSearchKey = key;

    this->m_nMaxPage = getMaxPage();
    searchData();
}

void SearchDialog::firstPageClicked()
{
    this->m_nCurrentPage = 1;
    searchData();
}
void SearchDialog::lastPageClicked()
{
    this->m_nCurrentPage = this->m_nMaxPage;
    searchData();
}
void SearchDialog::previousPageClicked()
{
    m_nCurrentPage--;
    if(m_nCurrentPage<1)
        m_nCurrentPage = 1;
    searchData();
}
void SearchDialog::nextPageClicked()
{
    m_nCurrentPage++;
    if(m_nCurrentPage > m_nMaxPage)
        m_nCurrentPage = m_nMaxPage;
    searchData();
}

void SearchDialog::itemDbClicked(QTreeWidgetItem *item,int column)
{
    FindDialog *pFindDialog = new FindDialog(this);
    pFindDialog->showDataById(item->text(0));
    pFindDialog->show();
}
