#include "searchlocal.h"
#include <QLayout>
#include <QDebug>

SearchLocal::SearchLocal(QWidget *parent) : QWidget(parent)
{
    //设置窗口大小，固定大小
    this->setFixedSize(600,600);
    this->setStyleSheet("background-color:white;");
    m_localResults.empty();
    //初始化窗口组件
    m_resultWidget = new QListWidget(this);
    m_tipLabel= new QLabel(this);
    m_btnClose= new QPushButton(this);

    m_tipLabel->setObjectName("tiplabel");
    m_resultWidget->setObjectName("resultwidget");
    m_btnClose->setObjectName("btnclose");
    m_btnClose->setIcon(QIcon(":/icon/res/close.png"));

    //设置主窗口部件布局
    m_tipLabel->setGeometry(0,0,570,30);
    m_btnClose->setGeometry(570,0,30,30);
    m_resultWidget->setGeometry(0,30,600,570);

    //设置m_tipLabel样式
    m_tipLabel->setFrameStyle(QFrame::NoFrame);
    m_tipLabel->setText(tr("本地搜索结果 >>>>"));
    m_tipLabel->setFont(QFont("幼圆",12));

    //设置m_resultWidget样式
    m_resultWidget->setStyleSheet("font:20px \"幼圆\";");
    m_resultWidget->setViewMode(QListView::ListMode);
    m_resultWidget->setFrameShape(QFrame::NoFrame);
    m_resultWidget->setFlow(QListView::TopToBottom);
    m_resultWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultWidget->setMovement(QListView::Static);
    m_resultWidget->setResizeMode(QListView::Adjust);
    m_resultWidget->setStyleSheet(tr("QListWidget::Item{width: 600px;" \
                                     "height:40px;" \
                                     "color:black;" \
                                     "font:'幼圆' 20px;" \
                                     "background-color:rgba(255,255,255,90);}"));
    connect(m_resultWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemclicked(int)));
    connect(m_btnClose,SIGNAL(clicked()),
            this,SLOT(close()));

}

void SearchLocal::addsonginfoItems(int cur)
{
    QString resultinfo=m_localResults.values()[cur];
    QString songName=resultinfo.section(":",0,0);
    QString artist=resultinfo.section(":",1,1);
    QString cover_image=resultinfo.section(":",-1,-1);
    QWidget* m_songInfoWidget=new QWidget(m_resultWidget);
    QLabel* m_titleLabel=new QLabel;
    QLabel* m_artistLabel=new QLabel;
    QLabel* m_pictureLabel=new QLabel;

    m_pictureLabel->setScaledContents(true);
    m_pictureLabel->setFixedSize(100,100);
    m_titleLabel->setFont(QFont("幼圆",12));
    m_artistLabel->setFont(QFont("幼圆",10));
    m_artistLabel->setStyleSheet("color:rgba(0,0,0,75);");

    //设置m_songInfoWidget布局
    QGridLayout *gl1=new QGridLayout;
    gl1->setGeometry(QRect(0,0,600,120));
    gl1->addWidget(m_pictureLabel,0,0,2,1,Qt::AlignLeft);
    gl1->addWidget(m_titleLabel,0,1,1,5,Qt::AlignLeft);
    gl1->addWidget(m_artistLabel,1,1,1,5,Qt::AlignLeft);
    m_songInfoWidget->resize(600,120);
    m_songInfoWidget->setLayout(gl1);

    m_titleLabel->setText(songName);
    m_artistLabel->setText(artist);
    m_pictureLabel->setPixmap(QPixmap(cover_image));

    QListWidgetItem *pItem=new QListWidgetItem;
    m_resultWidget->insertItem(cur,pItem);
    m_resultWidget->setItemWidget(pItem,m_songInfoWidget);
    pItem->setSizeHint(QSize(600,120));

}

void SearchLocal::on_searchReply(QMap<QString, QString> localResults)
{
    m_localResults=localResults;
    for(int i=0;i<m_localResults.keys().length();i++)
    {
        addsonginfoItems(i);
    }
    this->show();

}

void SearchLocal::on_itemclicked(int cur)
{
    qDebug()<<"clicked";
    emit resendSongUrl(m_localResults.keys()[cur]);
}


