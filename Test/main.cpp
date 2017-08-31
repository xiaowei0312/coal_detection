#include <QtGui/QApplication>
#include "searchdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    SearchDialog dialog;
    dialog.show();
    
    return a.exec();
}
