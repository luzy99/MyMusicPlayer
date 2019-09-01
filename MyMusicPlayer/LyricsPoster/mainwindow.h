#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include "lyriclabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
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

    //点击自定义按钮（姐妹留给你写了）
    void on_btnAddPicture_clicked();
    //点击自定义字体按钮
    void on_btnChangeFont_clicked();
    //点击保存按钮
    void on_btnSave_clicked();

//protected:
//    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::MainWindow *ui;
    QWidget *m_pPictureWidget;
    QListWidget *m_pPictureListWidget;
    QListWidget *m_pLyricsListWidget;
    QTabWidget *m_pTabWidegt;
    QWidget *m_pButtonWidget;
    QString m_Songid;
    QStringList m_LyricList;
    QLabel *m_LyricLabel;
    QPoint m_mouseStartPoint;
    QPushButton *m_pbtnAddPicture;
    QPushButton *m_pbtnChangeFont;
    QPushButton *m_pbtnSave;
    int m_index;//新加的记录已经有了几张图片，给自己加背景用的

};

#endif // MAINWINDOW_H
