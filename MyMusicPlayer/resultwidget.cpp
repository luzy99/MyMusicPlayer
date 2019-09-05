#include "resultwidget.h"
#include <QLayout>
#include<QFileInfo>

ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口大小，固定大小
    this->setFixedSize(600,600);
    this->setStyleSheet("background-color:white;");
    //初始化窗口组件
    m_tipLabel=new QLabel(this);
    m_btnClose=new QPushButton(this);
    m_resultWidget=new QListWidget(this);
    m_songInfoWidget=new QWidget;
    m_titleLabel=new QLabel("hh");
    m_artistLabel=new QLabel("hhh");
    m_pictureLabel=new QLabel("hhhh");
    m_songPicture=new QPixmap;

    //设置窗口部件名称
    m_tipLabel->setObjectName(tr("tiplabel"));
    m_btnClose->setObjectName(tr("btnclose"));
    m_resultWidget->setObjectName(tr("resultwidget"));
    m_songInfoWidget->setObjectName("songinfowidget");
    m_titleLabel->setObjectName("titlelabel");
    m_artistLabel->setObjectName("artistlabel");
    m_pictureLabel->setObjectName("picturelabel");

    //设置主窗口部件布局
    m_tipLabel->setGeometry(0,0,570,30);
    m_btnClose->setGeometry(570,0,30,30);
    m_resultWidget->setGeometry(0,30,600,570);

    //设置m_songInfoWidget布局
    QGridLayout *gl1=new QGridLayout;
    gl1->setGeometry(QRect(0,0,600,120));
    gl1->addWidget(m_pictureLabel,0,0,2,1,Qt::AlignLeft);
    gl1->addWidget(m_titleLabel,0,1,1,5,Qt::AlignRight);
    gl1->addWidget(m_artistLabel,1,1,1,5,Qt::AlignRight);
    m_songInfoWidget->setLayout(gl1);
    //    m_songInfoWidget->show();

    //设置m_tipLabel样式
    m_tipLabel->setFrameStyle(QFrame::NoFrame);
    m_tipLabel->setText(tr("全网搜索结果 >>>>"));
    m_tipLabel->setFont(QFont("幼圆",12));

    //设置m_btnClose样式


    //设置m_resultWidget样式
    m_resultWidget->setStyleSheet("font:20px \"幼圆\";");
    m_resultWidget->setViewMode(QListView::ListMode);
    m_resultWidget->setFrameShape(QFrame::NoFrame);
    m_resultWidget->setFlow(QListView::TopToBottom);
    m_resultWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultWidget->setMovement(QListView::Static);
    m_resultWidget->setResizeMode(QListView::Adjust);
    m_resultWidget->setStyleSheet(tr("QListWidget::Item{width: 600px;height:40px;color:black;font:'幼圆' 20px;background-color:rgba(255,255,255,90);}"));
    m_titleLabel->setFont(QFont("幼圆",12));
    m_artistLabel->setFont(QFont("幼圆",10));
    m_artistLabel->setStyleSheet("color:rgba(0,0,0,75);");

}

void ResultWidget::getOnlinePic(QString song_url)
{
    //song_url="http://p2.music.126.net/daZcHVIJicL3wXJWMIjAng==/7926379325753633.jpg";
    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl(song_url));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    manager->get(QNetworkRequest(request));
}

void ResultWidget::addsonginfoItems()
{
    for(int index=0;index<m_searchResult.keys().length();index++)
    {
        getOnlinePic(m_searchResult.values()[index].pic_url);
        m_titleLabel->setText(m_searchResult.values()[index].title);
        m_artistLabel->setText(m_searchResult.values()[index].artist);
        QRect rect=m_songInfoWidget->geometry();
        QPixmap ptosave=m_songInfoWidget->grab(rect);
        QListWidgetItem *pItem=new QListWidgetItem;
        pItem->setSizeHint(QSize(600,120));
        pItem->setBackground(QBrush(QPixmap(ptosave).scaled(pItem->sizeHint())));
        m_resultWidget->addItem(pItem);
        delete pItem;
        //m_resultWidget->setItemWidget(pItem,m_songInfoWidget);

    }
}

void ResultWidget::on_replyFinished(QNetworkReply *reply)
{

    QFileInfo fileInfo(reply->url().path());
    QString fileName=fileInfo.fileName();
    qDebug()<<"接上了"<<fileName;
    int nHttpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<nHttpCode;
    m_songPicture->loadFromData(reply->readAll());
    qDebug()<<m_songPicture;
    m_pictureLabel->setAutoFillBackground(true);
    m_pictureLabel->setPixmap(m_songPicture->scaled(QSize(100,100)));
}

void ResultWidget::on_searchReply(QMap<QString, SongInfo> searchResult)
{
    m_searchResult.empty();
    m_searchResult=searchResult;
    addsonginfoItems();
    m_resultWidget->show();
    m_songInfoWidget->show();
    this->show();
}
