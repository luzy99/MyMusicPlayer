#include "skinchange.h"
#include <QLayout>
#include <QBitmap>
#include <QPainter>
#include <QDebug>

SkinChange::SkinChange(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    this->setFixedSize(325,220);
    this->setStyleSheet("border: 1px solid rgb(25,25,25);"
                        "background: rgb(245,245,247);");

    //绘制圆角窗口
     QBitmap bmp(this->size());
     bmp.fill();
     QPainter p(&bmp);
     p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.drawRoundedRect(bmp.rect(),2,2);
     setMask(bmp);
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
    hl2->setSpacing(0);
    hl2->setContentsMargins(0,0,0,0);
    hl2->addWidget(m_pSkinList);
    this->setLayout(hl2);

    connect(m_pSkinList,SIGNAL(itemSelectionChanged()),
            this,SLOT(on_iTemClicked()));
}

void SkinChange::initQMap()
{
    //初始化选项
    m_Color.insert(1,QColor(55,55,55));
    m_ColorName.insert(1,"酷炫黑");
    m_Color.insert(2,QColor(36,51,77));
    m_ColorName.insert(2,"海军蓝");
    m_Color.insert(3,QColor(248,133,98));
    m_ColorName.insert(3,"元气橙");
    m_Color.insert(4,QColor(94,46,148));
    m_ColorName.insert(4,"基佬紫");
    m_Color.insert(5,QColor(50,177,108));
    m_ColorName.insert(5,"早苗绿");
    m_Color.insert(6,QColor(0,160,233));
    m_ColorName.insert(6,"知乎蓝");
}

void SkinChange::addItems()
{
    for(int i=1 ; i <= m_Color.keys().length() ; i++)
    {
        m_pSkinWid = new QWidget(m_pSkinList);
        QString tip=m_ColorName.value(i);
        QString img_path=QString(":/skin/res/skin%1.png").arg(i);

        m_pTipLab= new QLabel(m_pSkinWid);
        m_pTipLab->setText(tip);
        m_pTipLab->setFixedSize(80,20);
        m_pTipLab->setFont(QFont("微软雅黑",9));
        m_pTipLab->setStyleSheet("background-color:rgba(0,0,0,50);"
                                 "color:white;border: none;");
        m_pTipLab->setAlignment(Qt::AlignCenter);
        m_pTipLab->setGeometry(0,60,80,20);

        m_pSkinLab= new QLabel(m_pSkinWid);
        m_pSkinLab->setPixmap(QPixmap(img_path).scaled(80,80));
        m_pSkinLab->setFixedSize(80,80);
        m_pSkinLab->setGeometry(0,0,80,80);
        m_pSkinLab->setStyleSheet("border-radius: 2px;");

        m_pTipLab->raise();

        m_pSkinWid->setFixedSize(80,80);

        QListWidgetItem *pItem=new QListWidgetItem;
        m_pSkinList->addItem(pItem);
        m_pSkinList->setItemWidget(pItem,m_pSkinWid);
        pItem->setSizeHint(QSize(80,80));
    }
}

void SkinChange::on_iTemClicked()
{
    int index = m_pSkinList->currentRow()+1;
    QColor color = m_Color.value(index);
    //qDebug()<<color;
    emit colorChanged(color);
}
