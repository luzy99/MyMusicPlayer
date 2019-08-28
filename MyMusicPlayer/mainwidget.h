#ifndef MainWidget_H
#define MainWidget_H

/*************************************************************************************
 * 功能：实现自定义窗体的无边框与移动,以及自定义标题栏-用来显示窗体的图标、标题，以及控制窗体最小化、最大化/还原、关闭、缩放（仅支持windows平台）。
*************************************************************************************/

#include <QWidget>
#include <QMouseEvent>
#include <QUrl>
#include "titlebar.h" //包含“自定义标题栏”头文件
#include "musicplaybar.h" //包含“自定义音乐播放栏”头文件
#include "songlist.h" //包含"‘自定义歌单"的头文件

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void initSignalsAndSlots(); //初始化信号与槽
    void dragEnterEvent(QDragEnterEvent *event); //重写拖入事件,允许直接拖入歌曲
    void dropEvent(QDropEvent *event); //重写拖入后的事件处理，将歌曲加入歌单

signals:
    void windowChange();
    void changePlaylist(QUrl url,int behaviorIndex);  //改变播放列表的信号，第二个参数代表操作类型，1:增加
    void changeListSongs(QString fileName,int behaviorIndex); //改变歌单的信号，第二个参数代表操作类型，1:增加
    void playMusic(int SongIndex); //触发播放音乐

public slots:
    void resetGeometry();
private:
    Ui::MainWidget *ui;

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度
    titleBar *pTitleBar; //自定义标题栏
    MusicPlayBar *pMusicPlayBar; //自定义音乐播放栏
    SongList *pSongList; //自定义歌单
};

#endif // MainWidget_H
