#include "logutil.h"
#include <QDate>
#include <QMessageBox>

LogUtil *LogUtil::pObj = NULL;
LogUtil::LogUtil()
{ 
    
}

LogUtil *LogUtil::getInstance()
{
    if(!pObj)
        pObj = new LogUtil();
    return pObj;
}

void LogUtil::logInit(const QString &logSavedPath,unsigned int maxFileSize)
{
    QDate today = QDate::currentDate();
    this->logSavedPath = logSavedPath;
    this->maxFileSize = maxFileSize;
    this->logName = (logSavedPath + "/" + "%1-%2-%3.log")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'));
    currentFile.setFileName(logName);
    
    this->logName_error = (logSavedPath + "/" + "error-%1-%2-%3.log")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'));
    currentFile_error.setFileName(logName_error);
}

void LogUtil::logWrite_normal(const QString &text)
{
    QDate today = QDate::currentDate();
    
    //�ж��ļ���С�Ƿ񳬹�����
    if(currentFile.exists() && currentFile.size() > maxFileSize)
    {
        //�ر�֮ǰ���ļ�
        currentFile.close();
        
        //�������ļ�
        this->logName = (logSavedPath + "/" + "%1-%2-%3.log")
            .arg(today.year(),4,10,QLatin1Char('0'))
            .arg(today.month(),2,10,QLatin1Char('0'))
            .arg(today.day(),2,10,QLatin1Char('0'));
        currentFile.setFileName(this->logName);
        for(int i=0;!currentFile.exists();i++)
        {
            this->logName += ("_" + i);
            currentFile.setFileName(this->logName);
            if(i==5)    //Something errors.
                break;
        }
    }
    
    //�ж��ļ��Ƿ��
    if(!currentFile.isOpen())
    {
        if (!currentFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::warning(NULL,QString::fromLocal8Bit("����"),
                QString::fromLocal8Bit("��־�ļ�[") + this->logName + QString::fromLocal8Bit("]�򿪳���"));
            return;
        }
    }
    
    //д���ļ�
    QTextStream out(&currentFile);
    QString logText = QString("[%1-%2-%3] %4")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'))
        .arg(text);
    out << logText << "\n";
}

void LogUtil::logWrite_error(const QString &text)
{
    QDate today = QDate::currentDate();
    
    //�ж��ļ���С�Ƿ񳬹�����
    if(currentFile_error.exists() && currentFile_error.size() > maxFileSize)
    {
        //�ر�֮ǰ���ļ�
        currentFile_error.close();
        
        //�������ļ�
        this->logName_error = (logSavedPath + "/" + "error-%1-%2-%3.log")
            .arg(today.year(),4,10,QLatin1Char('0'))
            .arg(today.month(),2,10,QLatin1Char('0'))
            .arg(today.day(),2,10,QLatin1Char('0'));
        currentFile_error.setFileName(this->logName_error);
        for(int i=0;!currentFile_error.exists();i++)
        {
            this->logName += ("_" + i);
            currentFile_error.setFileName(this->logName_error);
            if(i==5)    //Something errors.
                break;
        }
    }
    
    //�ж��ļ��Ƿ��
    if(!currentFile_error.isOpen())
    {
        if (!currentFile_error.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::warning(NULL,QString::fromLocal8Bit("����"),
                QString::fromLocal8Bit("��־�ļ�[") + this->logName_error + QString::fromLocal8Bit("]�򿪳���"));
            return;
        }
    }
    
    //д���ļ�
    QTextStream out(&currentFile_error);
    QString logText = QString("[%1-%2-%3] %4")
        .arg(today.year(),4,10,QLatin1Char('0'))
        .arg(today.month(),2,10,QLatin1Char('0'))
        .arg(today.day(),2,10,QLatin1Char('0'))
        .arg(text);
    out << logText << "\n";
}

void LogUtil::logWrite(LogUtil::TYPE type,const QString &text)
{
    switch(type)
    {
    case LogUtil::NORMAL:
        logWrite_normal(text);
        break;
    case LogUtil::ERROR:
        logWrite_error(text);
        break;
    default:
        break;
    }
}

void LogUtil::logClose()
{
    //�ر��ļ�
    currentFile.close();
    currentFile_error.close();
}
