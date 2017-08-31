#include "dialog_add.h"
#include "ui_dialog_add.h"
#include <QDebug>
#include <QtGui>
#include <QList>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include "dbutil.h"
#include "logutil.h"

AddDialog::AddDialog(QWidget *parent)
        : QDialog(parent), ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    isMarked = false;
    updateNeedSave(false);
    ui->btn_print->setEnabled(false);
    
    QDate today = QDate::currentDate();
    QString todayStr = QString("%1-%2-%3")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'));
    
    ui->lineEdit_301->setText(todayStr);
    
    signalMapper = new QSignalMapper(this);
    this->editListInit();
    this->colListInit();
    
    //构造各个lineEdit之间依赖列表
    this->generateDependencies();
    
    //添加foucsin事件过滤
    this->addEventFilter();
    
    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(btn_add_clicked()));
    connect(ui->btn_print,SIGNAL(clicked()),this,SLOT(btn_print_clicked()));
    connect(ui->btn_exit,SIGNAL(clicked()),this,SLOT(btn_exit_clicked()));
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::updateNeedSave(bool flag)
{
    this->needSave = flag;
    if(this->needSave)
    {
        if(!this->windowTitle().endsWith("(*)"))
           this->setWindowTitle(this->windowTitle()+"(*)");
        ui->btn_add->setEnabled(true);
    }
    else
    {
        if(this->windowTitle().endsWith("(*)"))
            this->setWindowTitle(this->windowTitle().replace("(*)",""));
        ui->btn_add->setEnabled(false);
    }
}

void AddDialog::colListInit()
{
    colNameList  << "id"
        << "data_000" << "data_001" << "data_002" << "data_010" << "data_011" << "data_012"  
        << "data_020" << "data_021" << "data_022" << "data_030" << "data_031" << "data_032" << "data_100" 
        << "data_101" << "data_102" << "data_103" << "data_200" << "data_201" << "data_202" << "data_203" << "data_204" << "data_205" 
        << "data_210" << "data_211" << "data_212" << "data_213" << "data_214" << "data_215" << "data_220" << "data_221" << "data_222" 
        << "data_223" << "data_224" << "data_225" 
        << "data_300" << "data_301" << "data_302" << "data_303" << "data_304" << "data_305" << "data_306" ;
}

void AddDialog::editListInit()
{
    editList1<< ui->lineEdit_000 << ui->lineEdit_001 << ui->lineEdit_002 << ui->lineEdit_010 << ui->lineEdit_011
             << ui->lineEdit_012 << ui->lineEdit_020 << ui->lineEdit_021 << ui->lineEdit_022
             << ui->lineEdit_030 << ui->lineEdit_031 << ui->lineEdit_032
             << ui->lineEdit_100 << ui->lineEdit_101 << ui->lineEdit_102 << ui->lineEdit_103
             << ui->lineEdit_200 << ui->lineEdit_201 << ui->lineEdit_202 << ui->lineEdit_203 << ui->lineEdit_204 << ui->lineEdit_205
             << ui->lineEdit_210 << ui->lineEdit_211 << ui->lineEdit_212 << ui->lineEdit_213 << ui->lineEdit_214 << ui->lineEdit_215
             << ui->lineEdit_220 << ui->lineEdit_221 << ui->lineEdit_222 << ui->lineEdit_223 << ui->lineEdit_224 << ui->lineEdit_225;
    
    editList2 << ui->lineEdit_300<< ui->lineEdit_301<< ui->lineEdit_302 << ui->lineEdit_303
            << ui->lineEdit_304<< ui->lineEdit_305<< ui->lineEdit_306;
    
    QList<QLineEdit*>::iterator it;
    int i=0;
    for(it=editList1.begin();it!=editList1.end();it++)
    {
        (*it)->setValidator(
                new QRegExpValidator(
                        QRegExp("^([1-9]\\d{0,15}|0)(\\.\\d{1,4})?$"),this));
        connect(*it,SIGNAL(textChanged(const QString&)),signalMapper, SLOT(map()));
        signalMapper->setMapping(*it,i++);
    }
    for(it=editList2.begin();it!=editList2.end();it++)
    {
        connect(*it,SIGNAL(textChanged(const QString&)),signalMapper, SLOT(map()));
        signalMapper->setMapping(*it, i++);
    }
    
    connect(signalMapper, SIGNAL(mapped(int)),
             this, SLOT(edit_text_changed_mapper(int)));
    
    ui->lineEdit_303->setValidator(new QRegExpValidator(
                        QRegExp("^1(3|4|5|7|8)\\d{9}$"),this));
    ui->lineEdit_301->setValidator(new QRegExpValidator(
                        QRegExp("^\\d{4}(\\-|\\/|\\.)\\d{1,2}(\\-|\\/|\\.)\\d{1,2}$"),this));
}

void AddDialog::edit_text_changed_mapper(int index)
{
    QLineEdit *pLineEdit;
    int baseSize = editList1.size();
    if(index < baseSize)
        pLineEdit = editList1.at(index);
    else
        pLineEdit = editList2.at(index - baseSize);
    
    generateData(pLineEdit);
    clearMark(pLineEdit);
    updateNeedSave(true);
}

//已废弃被edit_text_changed_mapper替代
void AddDialog::edit_text_changed(const QString &text)
{
    clearMarks();
    updateNeedSave(true);
}

//生成依赖数据
void AddDialog::generateData(QLineEdit *pLineEdit)
{
    for(int i=0;i<dataDepList.size();i++)
    {
        DataDependency *pDataObj = dataDepList.value(i); 
        for(int j=0;j<pDataObj->depList.size();j++)
        {
            if(pDataObj->depList.value(j) == pLineEdit)
            {
                if(!pDataObj->depHasEmptyElement())
                {
                    double d = pDataObj->execFormula();
                    if(pDataObj->error)
                    {
                        pDataObj->pTarget->setText("error");
                        addMark(pDataObj->pTarget);
                    }
                    else
                    {
                        pDataObj->pTarget->setText(QString::number(d,'f',4));
                        clearMark(pDataObj->pTarget);
                    }
                    break;
                }
            }
        }
    }
}

//构造依赖列表
int AddDialog::generateDependencies()
{
    //Fixed: 此处应该读取配置文件为最佳
    //dataDepList
    QDomDocument doc;  
    QFile file("D:/QtSpace/CoalQualityDetection/config/config.xml");  
    QString error = "";  
    int row = 0, column = 0;  
    if (!file.open(QIODevice::ReadOnly)) return -2;  
  
    if(!doc.setContent(&file, false, &error, &row, &column)){  
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;  
        file.close();  
        return -1;  
    }  
    file.close();  
    
    QDomElement root = doc.documentElement(); 
    if(root.tagName() != "datalist")
        return -3;
    QDomNode node = root.firstChild();  
    while(!node.isNull()) {  //item loop
        DataDependency *dataDep = new DataDependency;
        QDomElement element = node.toElement(); // try to convert the node to an element.  
        if(element.tagName() == "item")
        {    
            QDomNode tmpNode = element.firstChild(); 
            while(!tmpNode.isNull()){
                QDomElement tmpElement = tmpNode.toElement();
                if(tmpElement.tagName()=="target")
                    dataDep->pTarget = editList1.value(tmpElement.firstChild().toElement().text().toInt());
                else if(tmpElement.tagName()=="depList")
                {
                    QDomNode tNode = tmpElement.firstChild();
                    while(!tNode.isNull()){
                        dataDep->depList.push_back(editList1.value(tNode.toElement().text().toInt()));
                        tNode = tNode.nextSibling();
                    }
                }
                else if(tmpElement.tagName()=="formula")
                    dataDep->formula = tmpElement.text().trimmed();
                tmpNode = tmpNode.nextSibling();
            }
            dataDepList.push_back(dataDep);
        }
        node = node.nextSibling();  
    }  
    return 0;  
}

void AddDialog::addEventFilter()
{
    for(int i=0;i<dataDepList.size();i++)
        dataDepList.value(i)->pTarget->installEventFilter(this);
}

void AddDialog::closeEvent ( QCloseEvent * event )
{
    if(this->needSave)
    {
        switch(QMessageBox::question(this,QString::fromLocal8Bit("关闭确认"),
            QString::fromLocal8Bit("数据尚未保存，确定要关闭吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
        {
        case QMessageBox::Ok:
            break;
        default:
             event->ignore();
            return;
        }
    }
    event->accept();
}

bool AddDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::FocusIn)
    {    
        for(int i=0;i<dataDepList.size();i++)
        {
            DataDependency *pData = dataDepList.value(i);
            if(watched == pData->pTarget)
            {
                if(pData->pTarget->text().trimmed().isEmpty())
                {
                    isMarked = true;
                    clearMarks();
                    pData->markEmptyDepElement();
                }
            }
        }
    }
    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void AddDialog::btn_exit_clicked()
{
    if(this->needSave)
    {
        switch(QMessageBox::question(this,QString::fromLocal8Bit("关闭确认"),
            QString::fromLocal8Bit("数据尚未保存，确定要关闭吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
        {
        case QMessageBox::Ok:
            break;
        default:
            return;
        }
    }
    this->close();
}

void AddDialog::btn_print_clicked()
{
    QString id = ui->lineEdit_300->text().trimmed();
    if(id.isEmpty())
    {
        QMessageBox::warning(this,QString::fromLocal8Bit("无效打印"),QString::fromLocal8Bit("未检测到可打印数据，请先执行添加或者查询操作"));
        return;
    }
    if(this->needSave)
    {
        switch(QMessageBox::question(this,QString::fromLocal8Bit("打印确认"),
            QString::fromLocal8Bit("检测到数据发生改变但未保存，确定要打印吗?"),QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel))
        {
        case QMessageBox::Ok:
            break;
        default:
            return;
        }
    }
    printData(id);
}

void AddDialog::clearMark(QLineEdit *pLineEdit)
{
    pLineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");  
}

void AddDialog::clearMarks()
{
    if(!isMarked)
        return;
    isMarked = false;
    QList<QLineEdit*>::iterator it;
    for(it = editList1.begin();it!=editList1.end();it++)
        (*it)->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");  
    for(it = editList2.begin();it!=editList2.end();it++)
        (*it)->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}"); 
}

void AddDialog::addMark(QLineEdit *pLineEdit)
{
    pLineEdit->setStyleSheet("QLineEdit{border:1px solid red }");  
    isMarked = true;
}

void AddDialog::addMarks()
{
    QList<QLineEdit*>::iterator it;
    for(it = editList1.begin();it!=editList1.end();it++)
        if((*it)->text().trimmed().isEmpty())
            (*it)->setStyleSheet("QLineEdit{border:1px solid red }");  
    for(it = editList2.begin();it!=editList2.end();it++)
        if((*it)->text().trimmed().isEmpty())
            (*it)->setStyleSheet("QLineEdit{border:1px solid red }"); 
    
    QRegExpValidator dateReg(QRegExp("^\\d{4}(\\-|\\/|\\.)\\d{1,2}(\\-|\\/|\\.)\\d{1,2}$"),this);
    QRegExpValidator phoneReg(QRegExp("^1(3|4|5|7|8)\\d{9}$"),this);
    int pos = 0;
    QString text = ui->lineEdit_301->text();
    if(dateReg.validate(text,pos) != QValidator::Acceptable)
        ui->lineEdit_301->setStyleSheet("QLineEdit{border:1px solid red }");  
    text = ui->lineEdit_303->text();
    if(phoneReg.validate(text,pos)!= QValidator::Acceptable)
        ui->lineEdit_303->setStyleSheet("QLineEdit{border:1px solid red }");  
    
    isMarked = true;
}

void AddDialog::btn_add_clicked()
{ 
    if(!this->saveOrUpdateData())
    {
        QMessageBox:: warning(this,QString::fromLocal8Bit("失败"),
            QString::fromLocal8Bit("数据保存失败"));
        return;
    }

    QMessageBox:: information(this,QString::fromLocal8Bit("成功"),
        QString::fromLocal8Bit("数据保存成功"));
    
    ui->lineEdit_300->setText(QString::number(DBUtil::getInstance()->getLastInsertId())); 
    
    ui->btn_print->setEnabled(true);
    ui->btn_add->setText(QString::fromLocal8Bit("更新"));
    updateNeedSave(false);
    addMarks();
}

bool AddDialog::saveOrUpdateData()
{
    QString id = ui->lineEdit_300->text().trimmed();
    if(id.isEmpty())
        return saveData();
    else
        return updateData(id);
}

bool AddDialog::updateData(const QString &id)
{
    colValList.clear();
    QList<QLineEdit*>::iterator it;
    colValList << id;
    for(it = editList1.begin();it!=editList1.end();it++)
    {
        QString tmp="",text = (*it)->text();
        if(!text.isEmpty())
            tmp = QString::number((*it)->text().toDouble(),'f',4);
        colValList << tmp;
    }
    for(it = editList2.begin();it!=editList2.end();it++)
        colValList << (*it)->text();
    
    QString expressions = ("id = " + id);
    return DBUtil::getInstance()->update("t_coal_detection",colNameList,colValList,expressions);
}

bool AddDialog::saveData()
{
    colValList.clear();
    colValList << NULL; //id
    QList<QLineEdit*>::iterator it;
    for(it = editList1.begin();it!=editList1.end();it++)
    {
        QString tmp="",text = (*it)->text();
        if(!text.isEmpty())
            tmp = QString::number((*it)->text().toDouble(),'f',4);
        colValList << tmp;
    }
    for(it = editList2.begin();it!=editList2.end();it++)
        colValList << (*it)->text();
    
    return DBUtil::getInstance()->insert("t_coal_detection",colNameList,colValList);
}

bool AddDialog::printData(const QString &id)
{
    qDebug() << "printData";
    return true;
}










