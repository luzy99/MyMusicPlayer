#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include "myprogressbar.h"
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWindow(QString id,QString name,QWidget *parent = nullptr);
    void initSignalsAndSlots();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:

private slots:
    void on_choosePathBtn_clicked();
    void on_downloadBtn_clicked();
    void onDownloadProgress(qint64 current ,qint64 total);

private:
    QString songId; //歌曲id
    QString songName; //歌曲名称
    QString savePath; //文件保存的路径

    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位

    QLabel *choosePathLabel; //选择路径的提示标签路径
    QLineEdit *choosePathEdit; //选择路径的输入框
    QPushButton *choosePathBtn; //选择路径的按钮
    QPushButton *downloadBtn; //下载的按钮

    QFile *file; //下载的临时文件
    QFileInfo fileInfo; //下载文件的信息

    MyProgressBar *downloadProgress; //自定义进度条
};

#endif // DOWNLOADWINDOW_H
