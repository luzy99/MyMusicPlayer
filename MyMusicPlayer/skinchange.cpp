#include "skinchange.h"
#include <QLayout>
#include <QDebug>

SkinChange::SkinChange(QWidget *parent) : QWidget(parent)
{
    initQMap();
    m_pSkinList=new QListWidget(this);

    m_pSkinList->setStyleSheet("background-color:rgba(255,255,255,100)");
    m_pSkinList->setResizeMode(QListView::Adjust);
    m_pSkinList->setFlow(QListView::LeftToRight);
    m_pSkinList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pSkinList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pSkinList->setViewMode(QListView::IconMode);
    m_pSkinList->setFrameShape(QFrame::NoFrame);
    m_pSkinList->setMovement(QListView::Static);
    m_pSkinList->setSpacing(20);
    addItems();

    QHBoxLayout *hl2=new QHBoxLayout;
    hl2->addWidget(m_pSkinList);
    hl2->setAlignment(Qt::AlignCenter);
    this->setLayout(hl2);

    this->setFixedSize(350,250);

    connect(m_pSkinList,SIGNAL(currentRowChanged(int)),this,SLOT(on_iTemClicked(int)));
}

void SkinChange::initQMap()
{
    m_SkinMap.insert("酷炫黑",QColor(25,25,25));
    m_SkinMap.insert("海军蓝",QColor(36,51,77));
    m_SkinMap.insert("元气橙",QColor(248,133,98));
    m_SkinMap.insert("基佬紫",QColor(94,46,148));
    m_SkinMap.insert("早苗绿",QColor(50,177,108));
    m_SkinMap.insert("知乎蓝",QColor(0,160,233));

}

void SkinChange::addItems()
{
    for(int i=0;i<m_SkinMap.keys().length();i++)
    {
        m_pSkinWid = new QWidget(m_pSkinList);
        QString tip=m_SkinMap.keys()[i];
        QColor color=m_SkinMap.values()[i];
        QString index=index.number(i);
        QString img_path=QString(":/skin/res/skin%1.png").arg(index);

        m_pTipLab= new QLabel(m_pSkinWid);
        m_pTipLab->setText(tip);
        m_pTipLab->setFixedSize(80,20);
        m_pTipLab->setFont(QFont("微软雅黑",10));
        m_pTipLab->setStyleSheet("background-color:rgba(0,0,0,50);color:white;");
        m_pTipLab->setGeometry(0,60,80,20);

        m_pSkinLab= new QLabel(m_pSkinWid);
        m_pSkinLab->setPixmap(QPixmap(img_path).scaled(80,80));
        m_pSkinLab->setFixedSize(80,80);
        m_pSkinLab->setGeometry(0,0,80,80);

        m_pSkinWid->setFixedSize(80,80);
        //        QVBoxLayout *hl1=new QVBoxLayout(m_pSkinWid);
        //        hl1->addWidget(m_pSkinLab);
        //        hl1->addWidget(m_pTipLab);
        //        hl1->setAlignment(Qt::AlignBottom);
        //        m_pSkinWid->setLayout(hl1);

        QListWidgetItem *pItem=new QListWidgetItem;
        m_pSkinList->addItem(pItem);
        m_pSkinList->setItemWidget(pItem,m_pSkinWid);
        pItem->setSizeHint(QSize(80,80));
    }

}

void SkinChange::on_iTemClicked(int index)
{
    //qDebug()<<index;
    QColor color=m_SkinMap.values()[index];
    emit colorChanged(color);
}
