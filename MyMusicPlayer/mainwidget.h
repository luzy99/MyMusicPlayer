#ifndef MainWidget_H
#define MainWidget_H

#include <QWidget>
#include <QMouseEvent>
#include <QUrl>
#include <QDir>
#include "titlebar.h" //包含“自定义标题栏”头文件
#include "musicplaybar.h" //包含“自定义音乐播放栏”头文件
#include "songlist.h" //包含"‘自定义歌单"的头文件
#include "lyricdownload.h" //包含“歌词下载”功能的头文件
#include "lyricwidget.h" //展示歌词的组件的头文件
#include "minilyrics.h" //底部歌词弹幕
#include "songinfo.h" //代表歌曲信息的头文件
#include "songinfoshow.h" //展示歌曲信息的头文件
#include "audiotag.h" //爬取歌曲信息的组件

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void createActions(); //创建信号
    void initSignalsAndSlots(); //初始化信号与槽
    void initDirectory(); //初始化文件夹
    void dragEnterEvent(QDragEnterEvent *event); //重写拖入事件,允许直接拖入歌曲
    void dropEvent(QDropEvent *event); //重写拖入后的事件处理，将歌曲加入歌单

signals:
    void windowChange();
    void changePlaylist(QUrl url,int behaviorIndex);  //改变播放列表的信号，第二个参数代表操作类型，1:增加
    void changeListSongs(QString fileName,int behaviorIndex); //改变歌单的信号，第二个参数代表操作类型，1:增加
    void playMusic(int SongIndex); //触发播放音乐

public slots:
    void onTranslateChanged(); //歌词的翻译状态改变
    void onUpdateAudioTag(QString currentFileName); //切歌时爬取作者和封面的槽函数
    void onShowLyricsBarrage(bool show); //显示&隐藏底部歌词弹幕

private:
    Ui::MainWidget *ui;

    TitleBar *titleBar; //自定义标题栏 T
    MusicPlayBar *musicPlayBar; //自定义音乐播放栏 T
    SongList *pSongList; //自定义歌单 F
    LyricDownload *lyricsDownloader; //歌词下载 T
    LyricWidget *lyricsShower; //歌词展示 T
    miniLyrics *lyricsBarrage; //歌词弹幕 T
    bool translate; //歌词是否翻译 T
    SongInfo *currentSongInfo; //当前播放的歌曲的信息 T
    SongInfoShow *infoShow; //歌曲信息显示  T
};

#endif // MainWidget_H
