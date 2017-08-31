#include "searchdialog.h"
#include <QtCore>

SearchDialog::SearchDialog(QWidget *parent)
        :QDialog(parent)
{
    m_pSearchLineEdit = new QLineEdit();
    QPushButton *pSearchButton = new QPushButton(this);
    
    pSearchButton->setCursor(Qt::PointingHandCursor);
    pSearchButton->setFixedSize(22, 22);
    pSearchButton->setToolTip(QString::fromLocal8Bit("搜索"));
    pSearchButton->setStyleSheet(QString("QPushButton{border-image:url(:/images/icon_search_normal); background:transparent;}") + 
                                         "QPushButton:hover{border-image:url(:/images/icon_search_hover)}" + 
                                         "QPushButton:pressed{border-image:url(:/images/icon_search_press)}");
    
    //防止文本框输入内容位于按钮之下
    //QMargins margins = m_pSearchLineEdit->textMargins();
    //m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
    //m_pSearchLineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入搜索内容"));
    
    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(pSearchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0, 0, 0, 0);
    m_pSearchLineEdit->setLayout(pSearchLayout);
    
    connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));
}

void SearchDialog::search()
{
    QString strText = m_pSearchLineEdit->text();
    if (!strText.isEmpty())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("搜索"), QString::fromLocal8Bit("搜索内容为%1").arg(strText));
    }
}
