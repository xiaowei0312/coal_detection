#include "dbutil.h"
#include <QMessageBox>
#include <QtCore>
#include "logutil.h"
#include <QDebug>

DBUtil *DBUtil::pObj = NULL;

DBUtil::DBUtil()
{
}

DBUtil *DBUtil::getInstance()
{
    if(pObj==NULL)
        pObj = new DBUtil();
    return pObj;
}

QSqlDatabase DBUtil::dbInit(const QString &driver,const QString &dbName,
            const QString &hostname,int port,const QString &username,const QString &password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(dbName);
    db.setUserName(username);
    db.setPassword(password);
    db.setHostName(hostname);
    db.setPort(port);
    return db;
}

QSqlDatabase DBUtil::getConnection()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isValid())
        db = dbInit();
    if(!db.isOpen())
        db.open();
    
    if(!db.isOpen())
    {
        QMessageBox::critical(NULL,
            QString::fromLocal8Bit("ϵͳ����"),
            QString::fromLocal8Bit("�޷������ݿ⣬ϵͳ�쳣�˳�"));
        LogUtil::getInstance()->logWrite(LogUtil::ERROR,
            QString::fromLocal8Bit("�޷������ݿ⣬ϵͳ�쳣�˳�:") + db.lastError().databaseText());
        qApp->quit();
    }
    return db;
}

/*
    ���뺯��.
    ����SQL�������.
*/
bool DBUtil::insert(const QString &table, QStringList &names, QStringList &values)
{
    if (names.size() != values.size())
        return false;
 
    QSqlQuery query(getConnection());
    QString sql = QString("insert into ") + table + QString("(");
 
    for (int i=0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        if (i != names.size() - 1)
            sql+=QString(",");
        else
            sql = sql + QString(")");
    }
    sql = sql + QString("values (");
 
    for (int i = 0; i < values.size(); i++)
    {
        if(values.value(i) == NULL)
            sql += "null";
        else
            sql += QString("'") + QString(values.value(i)).trimmed() + QString("'");
        if (i != values.size()-1)
            sql += QString(",");
    }
    sql = sql + QString(")");
 
    //qDebug() << sql;
    if (query.exec(sql))
    {
        LogUtil::getInstance()->logWrite(LogUtil::NORMAL,sql);
        return true;
    }
    else
    {
        LogUtil::getInstance()->logWrite(LogUtil::ERROR,query.lastError().databaseText() + " [" + sql + "]");
        return false;
    }
}
 
/*
    �޸ĺ���.
    ����SQL�޸����.
*/
bool DBUtil::update(const QString &table, QStringList &names, QStringList &values, QString &expression)
{
    if (names.size() != values.size())
        return false;
 
    //UPDATE ������ SET ������ = ��ֵ WHERE ������ = ĳֵ
    QSqlQuery query(getConnection());
    QString sql = QString("update ")+table+QString(" set ");
    for (int i = 0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        sql = sql + QString(" = '");
        sql = sql + QString(values.value(i)).trimmed();
        sql = sql + QString("'");
        if (i != names.size()-1)
            sql = sql + QString(" ,");
    }
 
    sql = sql + QString(" where ") + expression;
    
    //qDebug() << sql;
    if (query.exec(sql))
    {
        LogUtil::getInstance()->logWrite(LogUtil::NORMAL,sql);
        return true;
    }
    else
    {
        LogUtil::getInstance()->logWrite(LogUtil::ERROR,query.lastError().databaseText() + " [" + sql + "]");
        return false;
    }
}

/*
    ɾ������.
    ����SQLɾ�����.
*/
bool DBUtil::del(const QString &table, QString &expression)
{
    //DELETE FROM ������ WHERE ������ = ֵ
    QSqlQuery query(getConnection());
    QString sql = QString("delete from ") + table + QString(" where ") + expression;
 
    if (query.exec(sql))
        return true;
    else
        return false;
}

bool DBUtil::select(const QString &table,QStringList &names,QString &expression,QList<QStringList> &values)
{
    //DELETE FROM ������ WHERE ������ = ֵ
    QSqlQuery query(getConnection());
    
    QString sql = QString("select ");
    for (int i = 0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        if (i != names.size()-1)
            sql = sql + QString(" ,");
    }
    
    sql += (" from " + table + QString(" where ") + expression);
    
    qDebug() << sql;
    if (!query.exec(sql))
        return false;
 
    while (query.next())
    {
        QStringList list;
        for (int i = 0; i < names.size(); i++)
            list << query.value(i).toString();
        values << list;
    }
    return true;
}

 
void DBUtil::getValues(const QString &table, QStringList &values)
{
    QSqlQuery query(getConnection());
    QString sql = QString("select * from ") + table;
    query.exec(sql);
    while (query.next())
        values << query.value(0).toString();
}

int DBUtil::getLastInsertId()
{
    QSqlQuery query(getConnection());
    QString sql = "select last_insert_rowid()";
    query.exec(sql);
    query.next();
    return query.value(0).toInt();
}


void DBUtil::dbClose()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
        db.close();
    if(db.isValid())
        db.removeDatabase("QSQLITE");
}

