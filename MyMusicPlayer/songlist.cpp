#include "songlist.h"
#include "titlebar.h"
#include<QDebug>
SongList::SongList(QWidget *parent) : QWidget(parent)
{
    //数据成员初始化，各个窗口父子关系确认
    g_container = new QGroupBox (this);
    scrollSongs = new QScrollArea (g_container);
    scrollLists = new QScrollArea (g_container);
    scrollListsWidget = new QWidget(scrollLists);
    scrollSongsWidget = new QWidget (scrollSongs);
    listList = new QListWidget(scrollSongsWidget);
    listSongs = new QListWidget(scrollListsWidget);

    scrollLists->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollSongs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //各个窗口位置确认
    g_container->setGeometry(0, 0, 240 , 870);
    scrollLists->setGeometry(0, 0, 240, 290);
    scrollSongs->setGeometry(0, 290, 240, 580);

    scrollListsWidget->setGeometry(0, 0, 230, 0);
    scrollListsWidget->setMinimumSize(230, 0);

    scrollSongsWidget->setGeometry(0, 0, 230 , 0);
    scrollSongsWidget->setMinimumSize(230, 0);
    listList->setGeometry(0, 0, 230, 0);
    listSongs->setGeometry(0, 0, 230, 0);

    //让部件显示
    g_container->show();
    scrollLists->show();
    scrollSongs->show();
    scrollListsWidget->show();
    scrollSongsWidget->show();
    listList->show();
    listSongs->show();

    //设置滚轮样式
    scrollLists->setWidget(scrollListsWidget);
    scrollSongs->setWidget(scrollSongsWidget);
    scrollLists->setStyleSheet("QScrollBar:vertical"
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
    "}");

    scrollSongs->setStyleSheet("QScrollBar:vertical"
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
    "}");

    //设置两个滚轮区无边框
    scrollLists->setFrameShape(QFrame::NoFrame);
    scrollSongs->setFrameShape(QFrame::NoFrame);

}


//当窗口发生改变的时候，歌单部分按照比例缩放
void SongList::resetGeometry()
{
    QWidget *pWindow = this->window();
    QSize pWindowSize = pWindow->size();
    int sizeY = pWindowSize.height();
    int sizeX = pWindowSize.width();
    this->resize(sizeX , sizeY);
    g_container->setGeometry(0, 0, 240 , sizeY-30);
    scrollLists->setGeometry(0, 0, 240,  ((sizeY-30 ))/3);
    scrollSongs->setGeometry(0, ((sizeY-30 ))/3, 240, (2*(sizeY-30))/3);
}


