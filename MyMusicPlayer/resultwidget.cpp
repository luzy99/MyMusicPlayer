#include "resultwidget.h"
#include <QLayout>
#include <QByteArray>
#include <Qfile>

ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口大小，固定大小
    this->setFixedSize(600,600);
    //this->setStyleSheet("background-color:white;");
    //初始化窗口组件
    m_tipLabel=new QLabel(this);
    m_btnClose=new QPushButton(this);
    m_resultWidget=new QListWidget(this);
//    m_songInfoWidget=new QWidget;
//    m_titleLabel=new QLabel("hh");
//    m_artistLabel=new QLabel("hhh");
//    m_pictureLabel=new QLabel("hhhh");
    m_songPicture=new QPixmap;
    curindex=0;

    //设置窗口部件名称
    m_tipLabel->setObjectName(tr("tiplabel"));
    m_btnClose->setObjectName(tr("btnclose"));
    m_resultWidget->setObjectName(tr("resultwidget"));
//    m_songInfoWidget->setObjectName("songinfowidget");
//    m_titleLabel->setObjectName("titlelabel");
//    m_artistLabel->setObjectName("artistlabel");
//    m_pictureLabel->setObjectName("picturelabel");

    //设置主窗口部件布局
    m_tipLabel->setGeometry(0,0,570,30);
    m_btnClose->setGeometry(570,0,30,30);
    m_resultWidget->setGeometry(0,30,600,570);

//    //设置m_songInfoWidget布局
//    QGridLayout *gl1=new QGridLayout;
//    gl1->setGeometry(QRect(0,0,600,120));
//    gl1->addWidget(m_pictureLabel,0,0,2,1,Qt::AlignLeft);
//    gl1->addWidget(m_titleLabel,0,1,1,5,Qt::AlignRight);
//    gl1->addWidget(m_artistLabel,1,1,1,5,Qt::AlignRight);
//    m_songInfoWidget->setLayout(gl1);
////    m_songInfoWidget->show();

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
//    m_titleLabel->setFont(QFont("幼圆",12));
//    m_artistLabel->setFont(QFont("幼圆",10));
//    m_artistLabel->setStyleSheet("color:rgba(0,0,0,75);");

//    addsonginfoItems();
    connect(m_resultWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemclicked(int)));
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

void ResultWidget::addsonginfoItems(int cur)
{
    //getOnlinePic(m_searchResult.values()[index].pic_url);
    QWidget* m_songInfoWidget=new QWidget(m_resultWidget);
    QLabel* m_titleLabel=new QLabel("hh");
    QLabel* m_artistLabel=new QLabel("hhh");
    QLabel* m_pictureLabel=new QLabel("hhhh");
    QString curindex=curindex.number(cur);
    QString filepath="./song_picture/"+curindex+".png";
//    QPixmap* songPicture=new QPixmap(filepath);
//    qDebug()<<curindex;
//    qDebug()<<filepath;
//    qDebug()<<songPicture;

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
//    m_songInfoWidget->show();

//    getOnlinePic(m_searchResult.values()[index].pic_url);
    switch (m_mode)
    {
    case 0:
        m_titleLabel->setText(m_searchResult.values()[cur].title);
        m_artistLabel->setText(m_searchResult.values()[cur].artist);
        break;

    case 1:
        m_titleLabel->setText(m_mvResults.values()[cur]["title"]);
        m_artistLabel->setText(m_mvResults.values()[cur]["artist"]);
        break;
    }

    m_pictureLabel->setPixmap(QPixmap(filepath));
    qDebug()<<filepath;
    qDebug()<<QTime::currentTime();
    QListWidgetItem *pItem=new QListWidgetItem;
    m_resultWidget->addItem(pItem);
    m_resultWidget->setItemWidget(pItem,m_songInfoWidget);
    pItem->setSizeHint(QSize(600,120));
}

void ResultWidget::on_replyFinished(QNetworkReply *reply)
{

    m_songPicture->loadFromData(reply->readAll());
    //qDebug()<<m_songPicture;
//    m_pictureLabel->setAutoFillBackground(true);
//    m_pictureLabel->setPixmap(m_songPicture->scaled(QSize(100,100)));
    QString index=index.number(curindex);
    QString filepath="./song_picture/"+index+".png";
    if(!m_songPicture->save(filepath,"png"))

    {

        qDebug()<<"save widget screen failed"<<endl;

    }
    else
    {
        qDebug()<<"save";
        qDebug()<<QTime::currentTime();
        addsonginfoItems(curindex);
    }


    curindex++;
}

void ResultWidget::on_searchReply(QMap<QString, SongInfo> searchResult)
{
    m_mode=0;
    m_searchResult.empty();
    m_searchResult=searchResult;
    for(int index=0;index<m_searchResult.keys().length();index++)
    {
        getOnlinePic(m_searchResult.values()[index].pic_url);
    }
    //addsonginfoItems();
    m_resultWidget->show();
    //m_songInfoWidget->show();
    this->show();
}

void ResultWidget::on_searchReply1(QMap<QString, QMap<QString, QString> > mvResults)
{
    m_mode=1;
    m_mvResults.empty();
    m_mvResults=mvResults;
    for(int index=0;index<m_mvResults.keys().length();index++)
    {
        getOnlinePic(m_mvResults.values()[index]["pic_url"]);
    }
    //addsonginfoItems();
    m_resultWidget->show();
    //m_songInfoWidget->show();
    this->show();
}

void ResultWidget::on_itemclicked(int cur)
{
    qDebug()<<"clicked";
    switch (m_mode)
    {
    case 0:
        emit resendSongInfo(m_searchResult.values()[cur]);
        break;
    case 1:
        emit resendnvId(m_mvResults.keys()[cur]);
    }

}
