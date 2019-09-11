#include "addintosonglist.h"
#include <QDebug>
#include <QGridLayout>
#include <QSqlQuery>
#include <QBitmap>
#include <QPainter>

AddIntoSongList::AddIntoSongList(QString UserId,QWidget *parent)
    : QDialog(parent),
      User(UserId)
{
    listList = new QListWidget(this);
    closeButton = new QPushButton(this);
    title = new QLabel(this);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Background,QColor(245,245,247));
    this->setPalette(pal);
    this->setFixedSize(500,460);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    //绘制圆角窗口
     QBitmap bmp(this->size());
     bmp.fill();
     QPainter p(&bmp);
     p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.drawRoundedRect(bmp.rect(),2,2);
     setMask(bmp);
    //各个窗口位置确认
    closeButton->setGeometry(460, 0, 40, 40);
    closeButton->setFlat(true);
    closeButton->setIcon(QIcon(":/icon/res/titleClose.png"));
    closeButton->setStyleSheet("background: rgb(25,25,25);"
                               "border:none;");

    title->setGeometry(0, 0, 460, 40);
    title->setText("   添加到歌单");
    title->setFont(QFont("微软雅黑", 11));
    title->setStyleSheet("color: rgb(245,245,247);"
                         "background: rgb(25,25,25);"
                         "border:none;");

    listList->setGeometry(0, 40, 500, 420);
    listList->setMinimumSize(500,420);

    //消除滚轮区内部的窗口边框
    listList->setFrameShape(QFrame::NoFrame);

    //滚轮样式
    QString scrollStyleSheet = "QScrollBar:vertical"
                               "{   width:8px;  "
                               "   background:rgba(0,0,0,0%);"
                               "    margin:0px,0px,0px,0px;"
                                "   padding-top:9px;"
                                "   padding-bottom:9px;"
                              " }"
                               "QScrollBar::handle:vertical"
                               "{"
                                   "width:8px;"
                                  " background:rgba(0,0,0,25%);"
                                  " border-radius:4px;"
                                   "min-height:20;"
                               "}"
                               "QScrollBar::handle:vertical:hover"
                               "{"
                                  " width:8px;"
                                  " background:rgba(0,0,0,50%);"
                                  " border-radius:4px;"
                                  " min-height:20;"
                               "}"
                               "QScrollBar::add-line:vertical"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/3.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:vertical"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/1.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-line:vertical:hover"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/4.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:vertical:hover"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/2.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                               "{"
                                   "background:rgba(0,0,0,10%);"
                                   "border-radius:4px;"
                               "}"

                               "QScrollBar:horizontal"
                               "{"
                                   "width:8px;"
                                   "background:rgba(0,0,0,0%);"
                                   "margin:0px,0px,0px,0px;"
                                   "padding-top:9px;"
                                   "padding-bottom:9px;"
                               "}"
                               "QScrollBar::handle:horizontal"
                               "{"
                                  " width:8px;"
                                  " background:rgba(0,0,0,25%);"
                                  " border-radius:4px;"
                                   "min-height:20;"
                               "}"
                               "QScrollBar::handle:horizontal:hover"
                               "{"
                                   "width:8px;"
                                  " background:rgba(0,0,0,50%);"
                                   "border-radius:4px;"
                                  " min-height:20;"
                               "}"
                               "QScrollBar::add-line:horizontal"
                               "{"
                                  " height:9px;width:8px;"
                                  " border-image:url(:/images/a/3.png);"
                                  " subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:horizontal"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/1.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-line:horizontal:hover"
                              " {"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/4.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:horizontal:hover"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/2.png);"
                                   "subcontrol-position:left;"
                              " }"
                               "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal "
                               "{"
                                   "background:rgba(0,0,0,10%);"
                                   "border-radius:4px;"
                               "}";

    listList->setStyleSheet("QListWidget{color:rgb(92,92,92); background:rgb(245,245,247);border:0px solid gray;}" \
                            "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}" \
                            "QListWidget::Item:hover{color:rgb(55,57,61);background:rgb(230,231,234);border:0px solid gray;}" \
                            "QListWidget::Item:selected{color:rgb(0,0,0);background:rgb(230,231,234);border:none;}" \
                            "QListWidget:focus{outline:none;}");

    QSqlQuery query;
    QString cmd = "show tables;";
    QSqlQuery query1(cmd);
    while (query1.next())
    {
        if(processStringId(query1.value(0).toString()) == User)
        {
            QString tableName = QString(query1.value(0).toString());
            if(processStringName( tableName) == QString("播放历史"))
            {
                continue;
            }
            listInfoWidget = new QWidget();
            item_titleLabel=new QLabel();
            item_numLabel=new QLabel();
            item_songPicture=new QLabel();
            //设置m_songInfoWidget布局

            query.exec(QString("select count(*) from %1").arg(tableName));
            query.next();
            int totalSong = query.value(0).toInt();

            QGridLayout *gl1=new QGridLayout();
            QListWidgetItem *pItem = new QListWidgetItem();

            item_numLabel->setText(QString("%1首音乐").arg(totalSong));
            item_numLabel->setFont(QFont("微软雅黑", 9));
            item_numLabel->setStyleSheet("color:gray;");

            query.exec(QString("select cover_image from %1 as t order by t.num desc")
                       .arg(tableName));
            query.next();
            QPixmap pic(query.value(0).toString());
            item_songPicture->setPixmap(pic);
            item_songPicture->setScaledContents(true);
            item_songPicture->setFixedSize(60,60);
            gl1->setGeometry(QRect(0,0,500,60));
            gl1->addWidget(item_songPicture,0,0,2,1,Qt::AlignLeft);
            gl1->addWidget(item_titleLabel,0,1,1,5,Qt::AlignLeft);
            gl1->addWidget(item_numLabel,1,1,1,5,Qt::AlignLeft);

            listInfoWidget->setLayout(gl1);
            if(processStringName( tableName) == QString("我喜欢的音乐"))
            {
                listList->insertItem(0, pItem);
            }
            else
            {
                listList->addItem(pItem);
            }
            listList->setItemWidget(pItem,listInfoWidget);
            pItem->setSizeHint(QSize(500,70));
            pItem->setText(QString("                  %1\n").arg(processStringName(tableName)));
            pItem->setFont(QFont("微软雅黑", 11));
        }
    }

    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(closeWindow()));
    connect(listList, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(onItemClicked(QListWidgetItem *)));
}

void AddIntoSongList::closeWindow()
{
    this->done(0);
}

void AddIntoSongList::onItemClicked(QListWidgetItem * item)
{
    addedSongList = item->text().split("                  ")[1].split("\n")[0];
    this->done(1);
}

QString AddIntoSongList::getAddedSongList() const
{
    return addedSongList;
}

QString AddIntoSongList::connectString(QString listName)
{
    return User+QString("_")+listName;
}
QString AddIntoSongList::processStringId(QString tableName)
{
    return tableName.mid(0,11);
}
QString AddIntoSongList::processStringName(QString tableName)
{
    return tableName.mid(12,tableName.length()-12);
}

void AddIntoSongList::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}
void AddIntoSongList::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}
