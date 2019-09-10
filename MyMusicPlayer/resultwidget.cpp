#include "resultwidget.h"
#include <QLayout>
#include <QByteArray>
#include <QFileInfo>
#include <QEventLoop>
#include <QElapsedTimer>
#include <QDir>
#include <QBitmap>
#include <QPainter>
#include <thread>

ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口大小，固定大小
    this->setFixedSize(600,600);
    this->setStyleSheet("background-color:white;");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    this->setStyleSheet("border: 1px solid rgb(25,25,25);");
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    //绘制圆角窗口
     QBitmap bmp(this->size());
     bmp.fill();
     QPainter p(&bmp);
     p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.drawRoundedRect(bmp.rect(),2,2);
     setMask(bmp);

    //初始化窗口组件
    m_tipLabel=new QLabel(this);
    m_btnClose=new QPushButton(this);
    m_resultWidget=new QListWidget(this);
    m_songPicture=new QPixmap;
    curindex=0;

    //设置窗口部件名称
    m_tipLabel->setObjectName(tr("tiplabel"));
    m_btnClose->setObjectName(tr("btnclose"));
    m_btnClose->setIcon(QIcon(":/icon/res/titleClose.png"));
    m_btnClose->setStyleSheet("border: none;background: rgb(25,25,25);");
    m_resultWidget->setObjectName(tr("resultwidget"));
    //设置主窗口部件布局
    m_tipLabel->setGeometry(0,0,570,40);
    m_btnClose->setGeometry(570,0,30,40);
    m_resultWidget->setGeometry(0,40,600,560);

    //设置m_tipLabel样式
    m_tipLabel->setFrameStyle(QFrame::NoFrame);
    m_tipLabel->setStyleSheet("border:none;"
                              "color: rgb(245,245,247);"
                              "background: rgb(25,25,25);");
    m_tipLabel->setText("  全网搜索结果 >>>>");
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
    m_resultWidget->setStyleSheet("QListWidget{background:rgb(245,245,247);border:0px solid gray;}" \
                                  "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}" \
                                  "QListWidget::Item:hover{background:rgb(255,232,233);border:0px solid gray;}" \
                                  "QListWidget::Item:selected{background:rgb(230,231,234);border:none;}" \
                                  "QListWidget:focus{outline:none;}");

    //    addsonginfoItems();
    connect(m_resultWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemclicked(int)));
    connect(m_btnClose,SIGNAL(clicked()),
            this,SLOT(close()));
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
    QListWidgetItem *pItem=new QListWidgetItem;
    m_resultWidget->addItem(pItem);
    m_resultWidget->setItemWidget(pItem,
                                  addNewItemWidget(cur));
    pItem->setSizeHint(QSize(600,120));
}

void ResultWidget::addPicture(QString pid)
{
    int index;
    foreach (index, picMap.keys())
    {
        if(!picMap.value(index).compare(pid))//匹配所有该封面的item
        {
            m_resultWidget->setItemWidget(m_resultWidget->item(index),
                                          addNewItemWidget(index));
        }
    }
}

QWidget *ResultWidget::addNewItemWidget(int cur)
{
    //getOnlinePic(m_searchResult.values()[index].pic_url);
    QWidget* m_songInfoWidget=new QWidget(m_resultWidget);
    QLabel* m_titleLabel=new QLabel("hh");
    QLabel* m_artistLabel=new QLabel("hhh");
    QLabel* m_pictureLabel=new QLabel("hhhh");
    //QString curindex=QString::number(cur);
    QString pid=picMap.value(cur);
    QString filepath="./searchPictures/"+pid+".png";

    m_pictureLabel->setScaledContents(true);
    m_pictureLabel->setFixedSize(100,100);
    m_pictureLabel->setStyleSheet("border: none;");
    m_titleLabel->setFont(QFont("幼圆",12));
    m_titleLabel->setStyleSheet("border: none;");
    m_artistLabel->setFont(QFont("幼圆",10));
    m_artistLabel->setStyleSheet("color:rgba(0,0,0,75);border: none;");

    //设置m_songInfoWidget布局
    QGridLayout *gl1=new QGridLayout;
    gl1->setGeometry(QRect(0,0,600,120));
    gl1->addWidget(m_pictureLabel,0,0,2,1,Qt::AlignLeft);
    gl1->addWidget(m_titleLabel,0,1,1,5,Qt::AlignLeft);
    gl1->addWidget(m_artistLabel,1,1,1,5,Qt::AlignLeft);
    m_songInfoWidget->resize(600,120);
    m_songInfoWidget->setLayout(gl1);
    m_songInfoWidget->setStyleSheet("border: none;");
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
    QPixmap tempPic;
    if(!tempPic.load(filepath))
    {
        tempPic.load(":/icon/res/default_cover.png");
    }
    m_pictureLabel->setPixmap(tempPic);
    return m_songInfoWidget;
}

void ResultWidget::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}
void ResultWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

void ResultWidget::on_replyFinished(QNetworkReply *reply)
{
    m_songPicture->loadFromData(reply->readAll());

    QFileInfo picurl(reply->url().toString());
    QString pid=picurl.fileName().mid(0,picurl.fileName().length()-4);//图片id
    QString filepath="./searchPictures/"+pid+".png";
    //qDebug()<<pid;
    if(!m_songPicture->save(filepath,"png"))
    {
        qDebug()<<"save widget screen failed"<<endl;
    }
    else
    {
        qDebug()<<"save";
        //qDebug()<<QTime::currentTime();
        //addsonginfoItems(curindex);
        addPicture(pid);
        std::thread(addPicture);
    }

}

void ResultWidget::on_searchReply(QMap<QString, SongInfo> searchResult)
{
    m_mode=0;
    m_searchResult.clear();
    picMap.clear();
    m_searchResult=searchResult;
    for(int index=0;index<m_searchResult.keys().length();index++)
    {
        QFileInfo picurl(m_searchResult.values()[index].pic_url);
        QString pid=picurl.fileName().mid(0,picurl.fileName().length()-4);//图片id
        if(!picMap.values().contains(pid))//如果该图片未下载
        {
            getOnlinePic(m_searchResult.values()[index].pic_url);
            std::thread(getOnlinePic);
        }
        picMap.insert(index,pid);//index-图片id 对应关系
        addsonginfoItems(index);
    }
    //addsonginfoItems();
    m_resultWidget->show();
    //m_songInfoWidget->show();
    this->show();
}

void ResultWidget::on_searchReply1(QMap<QString, QMap<QString, QString> > mvResults)
{
    m_mode=1;
    m_tipLabel->setText("  MV搜索结果 >>>>");
    m_mvResults.clear();
    picMap.clear();
    m_mvResults=mvResults;
    for(int index=0;index<m_mvResults.keys().length();index++)
    {
        QFileInfo picurl(m_mvResults.values()[index].value("pic_url"));
        QString pid=picurl.fileName().mid(0,picurl.fileName().length()-4);//图片id
        if(!picMap.values().contains(pid))//如果该图片未下载
        {
            getOnlinePic(m_mvResults.values()[index].value("pic_url"));
            std::thread(getOnlinePic);
        }
        picMap.insert(index,pid);//index-图片id 对应关系
        addsonginfoItems(index);
    }
    //addsonginfoItems();
    m_resultWidget->show();
    //m_songInfoWidget->show();
    this->show();
}

void ResultWidget::on_itemclicked(int cur)
{
    qDebug()<<"clicked";
    if(m_mode == 0)
    {
        SongInfo selectedInfo = m_searchResult.values()[cur];
        QString picUrl = selectedInfo.pic_url;
        QFileInfo picurlInfo(picUrl);
        QString pid=picurlInfo.fileName().mid(0,picurlInfo.fileName().length()-4);//图片id
        QString newCoverPath = QDir::currentPath() + "/CoverImages/"+selectedInfo.title + ".png";
        QPixmap coverImage;
        qDebug()<<QDir::currentPath() + "/searchPictures/"+pid+".png";
        coverImage.load(QDir::currentPath() + "/searchPictures/"+pid+".png");
        coverImage.save(newCoverPath);
        selectedInfo.pic_url = newCoverPath;
        emit resendSongInfo(selectedInfo);//发送songinfo
        return;
    }
    else if(m_mode == 1)
    {
        emit resendnvId(m_mvResults.keys()[cur]);//发送mvid
        return;
    }
    else
    {

    }

}

void ResultWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->close();
}
