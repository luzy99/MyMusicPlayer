#ifndef MainWidget_H
#define MainWidget_H

/*************************************************************************************
 * 功能：实现自定义窗体的无边框与移动,以及自定义标题栏-用来显示窗体的图标、标题，以及控制窗体最小化、最大化/还原、关闭、缩放（仅支持windows平台）。
*************************************************************************************/

#include <QWidget>
#include <QMouseEvent>
#include "titlebar.h" //包含“自定义标题栏”头文件
#include "musicplaybar.h" //包含“自定义音乐播放栏”头文件

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

private:
    Ui::MainWidget *ui;

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度
    titleBar *pTitleBar; //自定义标题栏
    MusicPlayBar *pMusicPlayBar; //自定义音乐播放栏
};

#endif // MainWidget_H
