#include "downloadwindow.h"
#include <QHBoxLayout>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QEventLoop>
#include <QPalette>

DownloadWindow::DownloadWindow(QString id,QString name,QWidget *parent)
    : QWidget(parent),
      songId(id),
      songName(name)
{
    //修改下载框的样式
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(750,60); //悬浮窗大小不可改变
    this->setWindowOpacity(0.75);
    this->setStyleSheet("background-color: rgb(255,255,255);");

    //绘制圆角窗口
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),2,2);
    setMask(bmp);

    //初始化布局
    QHBoxLayout *layout = new QHBoxLayout;

    //初始化提示标签
    choosePathLabel = new QLabel;
    choosePathLabel->setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明
    choosePathLabel->setObjectName("choosePathLabel");
    choosePathLabel->setText("歌曲保存路径:");
    layout->addWidget(choosePathLabel);

    //初始化显示路径的输入框
    choosePathEdit = new QLineEdit;
    choosePathEdit->setObjectName("choosePathEdit");
    choosePathEdit->setText(QDir::currentPath()+"/Songs");
    choosePathEdit->setStyleSheet("background-color:rgba(255,229,238,1);");
    layout->addWidget(choosePathEdit);

    //初始化选择路径的按钮
    choosePathBtn = new QPushButton;
    choosePathBtn->setObjectName("choosePathBtn");
    choosePathBtn->setAttribute(Qt::WA_TranslucentBackground);
    choosePathBtn->setIcon(QIcon(":/icon/res/choosePath.png"));
    choosePathBtn->setIconSize(QSize(25,25));
    choosePathBtn->setFixedSize(QSize(30,30));
    choosePathBtn->setFlat(true);
    choosePathBtn->setToolTip("点击选择文件保存的路径");
    choosePathBtn->setStyleSheet("border: none;");
    choosePathBtn->setAttribute(Qt::WA_Hover,true);
    choosePathBtn->installEventFilter(this);
    layout->addWidget(choosePathBtn);

    //初始化下载按钮
    downloadBtn = new QPushButton;
    downloadBtn->setObjectName("downloadBtn");
    downloadBtn->setAttribute(Qt::WA_TranslucentBackground);
    downloadBtn->setIcon(QIcon(":/icon/res/download.png"));
    downloadBtn->setIconSize(QSize(25,25));
    downloadBtn->setFixedSize(QSize(30,30));
    downloadBtn->setFlat(true);
    downloadBtn->setToolTip("点击下载文件");
    downloadBtn->setStyleSheet("border: none;");
    downloadBtn->setAttribute(Qt::WA_Hover,true);
    downloadBtn->installEventFilter(this);
    layout->addWidget(downloadBtn);

    //初始化进度条
    downloadProgress = new MyProgressBar;
    downloadProgress->setObjectName("downloadProgress");

    this->setLayout(layout);

    //关联信号与槽
    initSignalsAndSlots();
}

//初始化信号与槽
void DownloadWindow::initSignalsAndSlots()
{
    //点下路径按钮时弹出路径选择
    connect(choosePathBtn,SIGNAL(clicked()),
            this,SLOT(on_choosePathBtn_clicked()));
    //点击下载按钮开始下载
    connect(downloadBtn,SIGNAL(clicked()),
            this,SLOT(on_downloadBtn_clicked()));
}

//实现鼠标拖动窗口
void DownloadWindow::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos(); //记录鼠标的初始位置
    windowsStartPoint = this->pos(); //记录窗口的初始位置
}

void DownloadWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

//事件过滤器,模仿鼠标按下效果
bool DownloadWindow::eventFilter(QObject *obj, QEvent *event)
{
    //"选择路径"按钮的事件过滤器
    if(obj->objectName()=="choosePathBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            choosePathBtn->setIcon(QIcon(":/icon/res/choosePathHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            choosePathBtn->setIcon(QIcon(":/icon/res/choosePath.png"));
        }
    }

    //"下载文件"按钮的事件过滤器
    if(obj->objectName() == "downloadBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            downloadBtn->setIcon(QIcon(":/icon/res/downloadHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            downloadBtn->setIcon(QIcon(":/icon/res/download.png"));
        }
    }

    return QWidget::eventFilter(obj,event);
}

//选择文件路径按钮按下时的槽函数
void DownloadWindow::on_choosePathBtn_clicked()
{
    QString saveDirectory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,
                                                                                  tr("选择文件的保存路径"),
                                                                                  QDir::currentPath()));
    if(!saveDirectory.isEmpty())
    {
        choosePathEdit->setText(saveDirectory);
    }
}

//下载按钮按下时的槽函数
void DownloadWindow::on_downloadBtn_clicked()
{
    //首先检验输入路径是否存在
    //路径正确,开始文件下载,否则返回
    savePath = choosePathEdit->text();
    if(!QFile::exists(savePath))
    {
        QMessageBox::warning(this,"警告","输入的文件夹不存在.",QMessageBox::Yes);
        return;
    }

    //根据songId拼接出下载地址
    QString baseUrl("http://music.163.com/song/media/outer/url?id=%1.mp3");
    QString songUrlString = baseUrl.arg(songId);
    qDebug()<<songUrlString;
    QUrl songUrl(songUrlString);
    if(!songUrl.isValid())
    {
        //如果这个Url无效
        QMessageBox::information(this,"错误","该歌曲Url失效，下载失败.",QMessageBox::Yes);
        return;
     }

    //创建request请求
    QNetworkRequest request;
    request.setUrl(songUrl);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true); //允许重定向
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36" \
                         "(KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36");
    request.setRawHeader("Referer","http://music.163.com/");
    //构造网络管理
    QNetworkAccessManager *manager=new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *reply = manager->get(request);
    downloadProgress->show();
    this->hide();

    //构造事件循环
    QEventLoop loop;
    connect(manager, &QNetworkAccessManager::finished,
            &loop, &QEventLoop::quit);
    //监测下载进度
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64 ,qint64)));
    loop.exec();
    //开启子事件循环
    //请求结束并下载完成后，退出子事件循环
    int statuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if(statuscode == 200)
    {
        QString filePath = savePath+"/"+songName+".mp3";
        file = new QFile(filePath);
        file->open(QIODevice::WriteOnly);
        file->write(reply->readAll());
    }
    else
    {
        QMessageBox::information(this,"错误","该歌曲下载权限受限.",QMessageBox::Yes);
    }
    downloadProgress->close();
}

//根据下载进度重绘进度条
void DownloadWindow::onDownloadProgress(qint64 current, qint64 total)
{
    int currentPrecent = current*100/total;
    downloadProgress->setPersent(currentPrecent);
}
