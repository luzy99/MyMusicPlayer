#include "showlocal.h"
#include <QDebug>
#include <QSqlQuery>

ShowLocal::ShowLocal(QWidget *parent) : QWidget(parent)
{

    m_pIconShowList = new QListWidget(this);
    m_pListShowList = new QListWidget(this);
    m_pLocalShowTab = new QTabWidget(this);
    //m_SongUrls.append(" ");

    m_pIconShowList->setStyleSheet("background:rgb(245,245,247);");
    m_pIconShowList->setResizeMode(QListView::Adjust);
    m_pIconShowList->setFlow(QListView::LeftToRight);
    m_pIconShowList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pIconShowList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pIconShowList->setViewMode(QListView::IconMode);
    m_pIconShowList->setFrameShape(QFrame::NoFrame);
    m_pIconShowList->setMovement(QListView::Static);
    m_pIconShowList->setSpacing(40);
    m_pIconShowList->setContentsMargins(0,5,0,5);

    m_pListShowList->setStyleSheet("background:rgb(245,245,247);");
    m_pListShowList->setViewMode(QListView::ListMode);
    m_pListShowList->setFrameShape(QFrame::NoFrame);
    m_pListShowList->setFlow(QListView::TopToBottom);
    m_pListShowList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListShowList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListShowList->setMovement(QListView::Static);
    m_pListShowList->setResizeMode(QListView::Adjust);

    m_pLocalShowTab->addTab(m_pIconShowList,"Mode 1");
    m_pLocalShowTab->addTab(m_pListShowList,"Mode 2");

    //m_pTabWidegt->setStyleSheet("background-color:rgba(0,0,0,0);");
    m_pLocalShowTab->setDocumentMode(true);
    QString tab_stylesheet=tr("QTabBar::tab{width: 100px;height:20px;"
                              "font:15px '微软雅黑';color:white;background-color:rbga(0,0,0,5);}");
    tab_stylesheet.append("QTabBar::tab:selected{color: rgba(0,0,0,50);"
                          "background-color:rbga(0,0,0,10);}");
    //tab_stylesheet.append("QTabWidget::pane{border:0px;top:-1px;}");
    m_pLocalShowTab->setStyleSheet(tab_stylesheet);

    QVBoxLayout *hb1= new QVBoxLayout;
    hb1->addWidget(m_pLocalShowTab);
    hb1->setSpacing(0);
    hb1->setContentsMargins(0,0,0,0);
    this->setLayout(hb1);

    connect(m_pIconShowList,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_ItemClick1(int)));
    connect(m_pListShowList,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_ItemClick2(int)));
}

void ShowLocal::on_ItemClick1(int currow)
{
    qDebug()<<currow;
    qDebug()<<m_SongUrls[currow];
    emit songUrl(m_SongUrls[currow]);
}

void ShowLocal::on_ItemClick2(int currow)
{
    qDebug()<<currow;
    qDebug()<<m_SongUrls[currow];
    emit songUrl(m_SongUrls[currow]);
}

//用户登陆时刷新页面内容
void ShowLocal::onUserLogin(QString userID)
{
    m_userid=userID;

    m_pIconShowList->clear();
    m_pListShowList->clear();

    QSqlQuery query;
    QString sql=QString("select songName,songUrl,likeOrNot,artist,cover_image from %1_播放历史").arg(m_userid);
    query.exec(sql);
    index=0;
    while(query.next())
    {
        m_pIconinfoWidget=new QWidget;
        m_pListinfoWidget=new QWidget;

        QString songName=query.value(0).toString();
        QString songUrl=query.value(1).toString();
        bool LikeorNot=query.value(2).toBool();
        QString artist=query.value(3).toString();
        QString cover_image=query.value(4).toString();

        m_SongUrls.append(songUrl);

        m_pPicLabel= new QLabel;
        m_pPicLabel->setPixmap(QPixmap(cover_image));
        m_pPicLabel->setScaledContents(true);
        m_pPicLabel->setFixedSize(160,160);

        m_pLikeLabel =new QLabel;
        if(LikeorNot)
        {
            m_pLikeLabel->setPixmap(QPixmap(":/icon/res/heart.ico"));
            m_pLikeLabel->setStyleSheet("background-color:rgba(255,255,255,0);");
            m_pLikeLabel->setScaledContents(true);
            m_pLikeLabel->setFixedSize(20,20);
        }
        else
        {
            m_pLikeLabel->setPixmap(QPixmap(":/icon/res/heart6.png"));
            m_pLikeLabel->setStyleSheet("background-color:rgba(255,255,255,0);");
            m_pLikeLabel->setScaledContents(true);
            m_pLikeLabel->setFixedSize(20,20);

        }
        m_pTitleLabel=new QLabel;
        m_pTitleLabel->setText(songName);
        m_pTitleLabel->setStyleSheet("background-color:rgba(255,255,255,0);");
        m_pTitleLabel->setFont(QFont("微软雅黑",10));

        m_pArtistLabel= new QLabel;
        m_pArtistLabel->setText(artist);
        m_pArtistLabel->setStyleSheet("background-color:rgba(255,255,255,0);");
        m_pArtistLabel->setFont(QFont("微软雅黑",10));

        m_pTitleLabelicon=new QLabel;
        m_pTitleLabelicon->setText(songName);
        m_pTitleLabelicon->setStyleSheet("background-color:rgba(255,255,255,0);");
        m_pTitleLabelicon->setFont(QFont("微软雅黑",10));

        m_pArtistLabelicon= new QLabel;
        m_pArtistLabelicon->setText(artist);
        m_pArtistLabelicon->setStyleSheet("background-color:rgba(255,255,255,0);");
        m_pArtistLabelicon->setFont(QFont("微软雅黑",10));

        //设置m_pIconinfoWidget样式
        QGridLayout *gl1=new QGridLayout();
        QListWidgetItem *pItem = new QListWidgetItem();
        gl1->addWidget(m_pPicLabel,0,0,16,18,Qt::AlignCenter);
        gl1->addWidget(m_pTitleLabel,16,0,1,18,Qt::AlignCenter);
        gl1->addWidget(m_pArtistLabel,17,0,1,18,Qt::AlignCenter);
        m_pIconinfoWidget->setLayout(gl1);
        m_pIconinfoWidget->setContentsMargins(0,0,0,0);
        m_pIconinfoWidget->setFixedSize(QSize(260,260));
        m_pIconinfoWidget->setStyleSheet("background-color:rgba(0,0,0,10);");
        //m_pIconinfoWidget->show();
        pItem->setSizeHint(QSize(240,240));
        m_pIconShowList->insertItem(index,pItem);
        m_pIconShowList->setItemWidget(pItem,m_pIconinfoWidget);
        m_pIconShowList->setStyleSheet("top-border:0px;");

        QGridLayout *gl2=new QGridLayout();
        QListWidgetItem *qItem= new QListWidgetItem();
        gl2->addWidget(m_pTitleLabelicon,0,0,1,1,Qt::AlignLeft);
        gl2->addWidget(m_pArtistLabelicon,0,1,1,1,Qt::AlignCenter);
        gl2->addWidget(m_pLikeLabel,0,2,1,1,Qt::AlignRight);
        m_pListinfoWidget->setLayout(gl2);
        m_pListinfoWidget->setContentsMargins(0,0,0,0);
        m_pListinfoWidget->setFixedHeight(40);
        if(index%2==0)
        {
            m_pListinfoWidget->setStyleSheet("background-color:rgba(0,0,0,10);");
        }
        else
        {
            m_pListinfoWidget->setStyleSheet("background-color:rgba(255,255,255,100);");
        }
        qItem->setSizeHint(QSize(width(),40));
        m_pListShowList->insertItem(index,qItem);
        m_pListShowList->setItemWidget(qItem,m_pListinfoWidget);
        m_pListShowList->setStyleSheet("top-border:0px;");

        index++;
    }
}
