#include "dialog_find.h"
#include "ui_dialog_find.h"
#include <QtGui>
#include <QDebug>
#include <QList>
#include "dbutil.h"
#include "logutil.h"
#include <QDomDocument>
#include <QTextDocument>
#include <QPrinter>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    isMarked = false;
    updateNeedSave(false);
    
    ui->btn_find->setEnabled(false);
    ui->btn_print->setEnabled(false);
    
    signalMapper = new QSignalMapper(this);
    this->editListInit();
    this->colListInit();
    
    //构造各个lineEdit之间依赖列表
    this->generateDependencies();
    
    //添加foucsin事件过滤
    this->addEventFilter();
    
    connect(ui->btn_find,SIGNAL(clicked()),this,SLOT(btn_find_clicked()));
    connect(ui->btn_update,SIGNAL(clicked()),this,SLOT(btn_update_clicked()));
    connect(ui->btn_print,SIGNAL(clicked()),this,SLOT(btn_print_clicked()));
    connect(ui->btn_exit,SIGNAL(clicked()),this,SLOT(btn_exit_clicked()));
    
    connect(ui->lineEdit_searchkey,SIGNAL(textChanged(const QString &)),this,SLOT(searchkey_text_changed(const QString &)));
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::colListInit()
{
    colNameList  << "id"
        << "data_000" << "data_001" << "data_002" << "data_010" << "data_011" << "data_012"  
        << "data_020" << "data_021" << "data_022" << "data_030" << "data_031" << "data_032" << "data_100" 
        << "data_101" << "data_102" << "data_103" << "data_200" << "data_201" << "data_202" << "data_203" << "data_204" << "data_205" 
        << "data_210" << "data_211" << "data_212" << "data_213" << "data_214" << "data_215" << "data_220" << "data_221" << "data_222" 
        << "data_223" << "data_224" << "data_225" 
        << "data_300" << "data_301" << "data_302" << "data_303" << "data_304" << "data_305" << "data_306" ;
    colNameList_print  
            << "id" << "data_303" << "data_301"  << "data_304" 
            << "add_time" << "data_302" << "data_305" << "data_306" 
            << "data_200" << "data_210" << "data_220" 
            << "data_201" << "data_211" << "data_221" 
            << "data_202" << "data_212" << "data_222" 
            << "data_203" << "data_213" << "data_223" 
            << "data_204" << "data_214" << "data_224" 
            << "data_205" << "data_215" << "data_225";
}

void FindDialog::editListInit()
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
        (*it)->setEnabled(false);
        (*it)->setValidator(
                new QRegExpValidator(
                        QRegExp("^([1-9]\\d{0,15}|0)(\\.\\d{1,4})?$"),this));
        connect(*it,SIGNAL(textChanged(const QString&)),signalMapper, SLOT(map()));
        signalMapper->setMapping(*it,i++);
    }
    
    for(it=editList2.begin();it!=editList2.end();it++)
    {
        (*it)->setEnabled(false);
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

//生成依赖数据
void FindDialog::generateData(QLineEdit *pLineEdit)
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
int FindDialog::generateDependencies()
{
    //dataDepList
    QDomDocument doc;  
    QFile file(":/config/config.xml");
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

void FindDialog::addEventFilter()
{
    for(int i=0;i<dataDepList.size();i++)
        dataDepList.value(i)->pTarget->installEventFilter(this);
}

void FindDialog::updateNeedSave(bool flag)
{
    this->needSave = flag;
    if(this->needSave)
    {
        if(!this->windowTitle().endsWith("(*)"))
           this->setWindowTitle(this->windowTitle()+"(*)");
        ui->btn_update->setEnabled(true);
    }
    else
    {
        if(this->windowTitle().endsWith("(*)"))
        this->setWindowTitle(this->windowTitle().replace("(*)",""));
        ui->btn_update->setEnabled(false);
    }
}

bool FindDialog::eventFilter(QObject *watched, QEvent *event)
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

void FindDialog::clearMark(QLineEdit *pLineEdit)
{
    pLineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");  
}

void FindDialog::clearMarks()
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

void FindDialog::addMark(QLineEdit *pLineEdit)
{
    pLineEdit->setStyleSheet("QLineEdit{border:1px solid red }");  
    isMarked = true;
}

void FindDialog::addMarks()
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

void FindDialog::edit_text_changed_mapper(int index)
{
    QLineEdit *pLineEdit;
    int baseSize = editList1.size();
    if(index < baseSize)
        pLineEdit = editList1.at(index);
    else
        pLineEdit = editList2.at(index - baseSize);
    
    clearMark(pLineEdit);
    updateNeedSave(true);
    generateData(pLineEdit);
}

void FindDialog::edit_text_changed(const QString &text)
{
    clearMarks();
    updateNeedSave(true);
}

void FindDialog::searchkey_text_changed(const QString &text)
{
    if(!text.trimmed().isEmpty())
        ui->btn_find->setEnabled(true);
}

void FindDialog::changeEvent(QEvent *e)
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

void FindDialog::closeEvent(QCloseEvent *event)
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

void FindDialog::btn_exit_clicked()
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

void FindDialog::btn_print_clicked()
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

void FindDialog::showDataById(const QString &id)
{
    QList<QStringList> values;
    QString expressions = QString("id = %1 and is_deleted = 0").arg(id);
    if(!DBUtil::getInstance()->select("t_coal_detection",colNameList,expressions,values))
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("查找失败，请检查id格式是否正确"));
        return;
    }
    if(values.size()==0)
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("没有找到对应数据"));
        return;
    }
    
    for(int i=0;i<editList1.size();i++)
    {
        editList1.value(i)->setText(values[0].value(i+1));
        editList1.value(i)->setEnabled(true);
    }
    
    for(int i=0;i<editList2.size();i++)
    {
        editList2.value(i)->setText(values[0].value(editList1.size()+ i + 1));
        editList2.value(i)->setEnabled(true);
    }
    
    ui->lineEdit_300->setText(values[0].value(0));
    ui->lineEdit_300->setEnabled(false);
    updateNeedSave(false);
    ui->btn_print->setEnabled(true);
    ui->btn_find->setEnabled(false);
}

void FindDialog::btn_find_clicked()
{
    QString searchKey = ui->lineEdit_searchkey->text().trimmed();
    if(searchKey.isEmpty())
        return;
    showDataById(searchKey);
}

void FindDialog::btn_update_clicked()
{
    QString id = ui->lineEdit_300->text().trimmed();
    if(id.isEmpty())
    {
        QMessageBox:: warning(this,QString::fromLocal8Bit("无效更新"),
            QString::fromLocal8Bit("未检测到需要更新的数据"));
        return;
    }
    if(!this->updateData(id))
    {
        QMessageBox:: warning(this,QString::fromLocal8Bit("失败"),
            QString::fromLocal8Bit("数据保存失败"));
        return;
    }

    QMessageBox:: information(this,QString::fromLocal8Bit("成功"),
        QString::fromLocal8Bit("数据保存成功"));
    
    updateNeedSave(false);
    addMarks();
}

bool FindDialog::updateData(const QString &id)
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

bool FindDialog::printData(const QString &id)
{   
    QPrinter printer;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&printer, this);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printPreview(QPrinter*)));
    preview.exec();
    
    return true;
}

void FindDialog::printPreview(QPrinter *printer)
{
    QString id = ui->lineEdit_300->text().trimmed();    //btn_print
    QList<QStringList> values;
    QString expressions = QString("id = %1 and is_deleted = 0").arg(id);
    
    //从数据库中加载数据
    if(!DBUtil::getInstance()->select("t_coal_detection",colNameList_print,expressions,values))
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("查找失败，请检查id格式是否正确"));
        return;
    }
    if(values.size()==0)
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("查找失败"),
            QString::fromLocal8Bit("没有找到对应数据"));
        return;
    }
    
    
    //读取打印模板文件
    QFile file(":/config/print.html");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox:: critical(this,QString::fromLocal8Bit("打印失败"),
            QString::fromLocal8Bit("没有找到打印模板"));
        return;
    }
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    QString szPrintTemplate,line;
    while(1) 
    {
        line = stream.readLine();
        if(line.isNull())
            break;
        szPrintTemplate += line;
    }    
    
//    //读取css文件 //css文件没有产生效果
//    file.setFileName(":/config/print.css");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QMessageBox:: critical(this,QString::fromLocal8Bit("打印失败"),
//            QString::fromLocal8Bit("没有找到打印模板"));
//        return;
//    }
//    stream.setDevice(&file);
//    stream.setCodec(QTextCodec::codecForName("UTF-8"));
//    QString szCss;
//    while(1) 
//    {
//        line = stream.readLine();
//        if(line.isNull())
//            break;
//        szCss += line;
//    }  
    
    
    //填充数据

    QDateTime now = QDateTime::currentDateTime();
    QString nowstr = now.toString("yyyy-MM-dd hh:mm:ss");
    szPrintTemplate = szPrintTemplate.arg(values[0].value(0)).arg(nowstr);
    for(int i=1;i<values[0].size();i++)
    {
        if(i==4)
            szPrintTemplate = szPrintTemplate.arg(((QString)values[0].value(i)).split(' ')[0]);
        else    
            szPrintTemplate = szPrintTemplate.arg(values[0].value(i));
    }
    //打印
    QTextDocument textDocument;
    textDocument.setHtml(szPrintTemplate);
    //textDocument.setDefaultStyleSheet(szCss);
    textDocument.print(printer);
}

