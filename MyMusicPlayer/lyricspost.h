//这是歌词海报
#ifndef LYRICSPOST_H
#define LYRICSPOST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>

class LyricsPost : public QWidget
{
    Q_OBJECT
public:
    explicit LyricsPost(QString,QString,QWidget *parent = nullptr);
    void analyzeLyrics(QString);

signals:
    void BkPictureChanged(QString);

public slots:
    //void on_BkPictureChanged(QString);
    //改变背景图片
    void on_itemClicked1(int);
    //改变上面的歌词
    void on_itemClicked2(int);
    //用来接收主窗口发出的信号，信号内容为歌曲ID
    void on_shareclicked(QString);

    //点击自定义按钮
    void on_btnAddPicture_clicked();
    //点击自定义字体按钮
    void on_btnChangeFont_clicked();
    //点击保存按钮
    void on_btnSave_clicked();

private:
    QWidget *m_pPictureWidget;
    QListWidget *m_pPictureListWidget;
    QListWidget *m_pLyricsListWidget;
    QTabWidget *m_pTabWidegt;
    QWidget *m_pButtonWidget;
    QString m_Songid;
    QString m_AlbumCover;
    QStringList m_LyricList;
    QLabel *m_LyricLabel;
    QPoint m_mouseStartPoint;
    QPushButton *m_pbtnAddPicture;
    QPushButton *m_pbtnChangeFont;
    QPushButton *m_pbtnSave;
    QPushButton *m_pbtnClose;
    int m_index;//新加的记录已经有了几张图片，给自己加背景用的
};

#endif // LYRICSPOST_H
