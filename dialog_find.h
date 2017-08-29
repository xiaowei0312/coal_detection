#ifndef DIALOG_FIND_H
#define DIALOG_FIND_H

#include <QtGui/QDialog>

namespace Ui {
    class FindDialog;
}

class FindDialog : public QDialog {
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FindDialog *m_ui;
};

#endif // DIALOG_FIND_H
