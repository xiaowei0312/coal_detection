#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent ( QCloseEvent * event );

private:
    Ui::MainWindow *ui;
private slots:
    void addDetectionSlot();
    void findDetectionSlot();
    void systemExitSlot();
};

#endif // MAINWINDOW_H
