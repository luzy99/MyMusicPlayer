#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStackedWidget>
#include "titlebar.h" //包含“自定义标题栏”头文件
#include "suspensionwindow.h" //包含悬浮窗窗体的头文件
#include "musicplaybar.h" //包含“自定义音乐播放栏”头文件
#include "songlist.h" //包含"‘自定义歌单"的头文件
#include "createsonglistdialog.h" //包含"新建歌单"弹出框的头文件
#include "lyricdownload.h" //包含“歌词下载”功能的头文件
#include "lyricwidget.h" //展示歌词的组件的头文件
#include "minilyrics.h" //底部歌词弹幕
#include "songinfo.h" //代表歌曲信息的头文件
#include "songinfoshow.h" //展示歌曲信息的头文件
#include "audiotag.h" //爬取歌曲信息的组件
#include "littlesongbar.h" //首页地下小的歌曲标签
#include "animatedwallwg.h" //动画滚轮
#include "searchdata.h" //搜索框对应的搜索器

enum clickedEventType {No=0, Move, Resize }; //标明鼠标按下的事件类型

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void initDirectory(); //初始化文件夹
    void initSignalsAndSlots(); //初始化信号与槽
    void updateDragRegion(); //更新拖动区域
    void GaussianBlurUpdate(); //高斯模糊并将其结果跟新至背景图

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event); //重写拖入事件,允许直接拖入歌曲
    void dropEvent(QDropEvent *event); //重写拖入后的事件处理，将歌曲加入歌单

signals:
    void playMusic(int SongIndex); //触发播放音乐
    void sendSongListName(QString);//传递创建歌单的名字
    void songChanged(QString songId,bool translate); //触发歌词爬取
    void addIntoPlayList(QString);//添加到播放列表中
    void clearMusic();

private slots:
    //这里是接受标题栏信号的槽,主要是对窗体属性的一些操作
    void onShowSuspensionWindow();
    void onMinimizeWindow();
    void onMaximizeWindow();
    void onCloseWindow();

    //这里接受悬浮窗对主窗体发出的一些信号
    void onHideSuspensionWindow();

    //这是对歌单的一些操作
    void showCreateSongListDialog();

    //这是对显示组件的一些更新操作
    void onTranslateChanged(); //歌词的翻译状态改变
    void onRecieveSongInfo(SongInfo *info); //接受歌单爬取的歌曲信息并更新至主页面
    void onShowLyricsBarrage(bool show); //显示&隐藏底部歌词弹幕

    //这是切换界面的槽函数
    void onChangePage(int index);

private:
    TitleBar *titleBar; //自定义标题栏
    QStackedWidget *mainPageContainer; //主要界面的容器，实现界面切换
    MusicPlayBar *musicPlayBar; //自定义音乐播放栏
    SuspensionWindow *suspensionWindow; //小窗化窗体
    miniLyrics *lyricsBarrage; //歌词弹幕

    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位
    QRect dragRegion;
    clickedEventType clickedType; //事件类型

    LyricDownload *lyricsDownloader; //歌词下载
    SongInfo *currentSongInfo; //当前播放的歌曲的信息
    bool translate; //歌词是否翻译
    SearchData *searcher; //搜索框的逻辑的部件

    QWidget *songListPage; //展示歌单信息页面
    SongList *songList; //歌单列表
    LittleSongBar *littleSongBar; //底部小的歌曲信息展示
    AnimatedWallWG *adsWall; //动画滚轮

    QWidget *songInfoPage; //展示歌曲信息页面
    SongInfoShow *infoShow; //歌曲信息显示
     LyricWidget *lyricsShower; //歌词展示
};

#endif // MAINWINDOW_H
