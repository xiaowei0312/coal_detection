#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include "mainwindow.h"
#include "logutil.h"
#include "dbutil.h"
#include <QDebug>
#include "dialog_search.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QObject::connect(&a,SIGNAL(aboutToQuit()),LogUtil::getInstance(),SLOT(logClose()));
    QObject::connect(&a,SIGNAL(aboutToQuit()),DBUtil::getInstance(),SLOT(dbClose()));
    
    LogUtil::getInstance()->logInit(/* "./log",5*1024*1024 */);
    DBUtil::getInstance()->dbInit(/* "QSQLITE","D:\coal.db","",0,"","" */);
    
    MainWindow w;
    w.move((QApplication::desktop()->width() - w.width())/2,
           (QApplication::desktop()->height() - w.height())/2);
    w.show();
    
//     SearchDialog *pSearchDialog = new SearchDialog();
//     pSearchDialog->show();
    return a.exec();
}
