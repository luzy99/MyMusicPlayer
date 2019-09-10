#include "lyricspost.h"
#include "errorwindow.h"
#include "informationwindow.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLayout>
#include <QMouseEvent>
#include <QFontDialog>
#include <QFileDialog>

LyricsPost::LyricsPost(QString songId,QWidget *parent)
    :QWidget (parent),
     m_Songid(songId)
{
    //设置窗口大小
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(600,1000);

    //初始化窗口组件
    m_pPictureWidget =new QWidget(this);
    m_pTabWidegt=new QTabWidget(this);
    m_pPictureListWidget=new QListWidget(m_pPictureWidget);
    m_pLyricsListWidget=new QListWidget(m_pPictureWidget);
    m_LyricLabel=new QLabel(m_pPictureWidget);
    m_pButtonWidget=new QWidget(this);
    m_pbtnAddPicture=new QPushButton;
    m_pbtnChangeFont=new QPushButton;
    m_pbtnSave=new QPushButton;
    m_pbtnClose=new QPushButton;
    m_index=0;

    //设置事件过滤器
    m_LyricLabel->installEventFilter(this);

    //开始分析歌词
    this->analyzeLyrics(songId);

    //设置窗口部件名称
    m_pPictureWidget->setObjectName("pictureshow_Widget");
    m_pPictureListWidget->setObjectName("picturechose_Widget");
    m_pTabWidegt->setObjectName("tabWidget");
    m_pLyricsListWidget->setObjectName("liyricchose_Widget");
    m_LyricLabel->setObjectName("lyric_label");
    m_pButtonWidget->setObjectName("button_Widget");
    m_pbtnAddPicture->setObjectName("Btn_addpicture");
    m_pbtnChangeFont->setObjectName("Btn_changefont");
    m_pbtnSave->setObjectName("Btn_save");
    m_pbtnClose->setObjectName("Btn_close");

    //设置窗口布局
    m_pPictureWidget->setGeometry(0,0,600,600);
    m_pTabWidegt->setGeometry(0,555,600,397);
    m_LyricLabel->setGeometry(0,290,600,20);
    m_pButtonWidget->setGeometry(0,952,600,48);

    //设置m_pPictureWidget组件大小样式
    QHBoxLayout* lyriclayout=new QHBoxLayout;
    lyriclayout->setGeometry(QRect(0,0,600,2));
    lyriclayout->addWidget(m_LyricLabel);
    m_pPictureWidget->setLayout(lyriclayout);
    m_pPictureWidget->setAutoFillBackground(true);
    QPalette palette = m_pPictureWidget->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/providedPosts/res/1.JPG").scaled(m_pPictureWidget->size(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation)));// 使用平滑的缩放方式
    m_pPictureWidget->setPalette(palette);// 给widget加上背景图


    //设置tabwidget窗口的样式
    m_pTabWidegt->addTab(m_pPictureListWidget,"背景");
    m_pTabWidegt->addTab(m_pLyricsListWidget,"歌词");
    //m_pTabWidegt->setStyleSheet("background-color:rgba(0,0,0,0);");
    m_pTabWidegt->setDocumentMode(true);
    QString tab_stylesheet=tr("QTabBar::tab{width: 300px;height:46px;font:20px '幼圆';color:black;background-color:rbga(0,0,0,75);}");
    tab_stylesheet.append("QTabBar::tab:selected{color: white;background-color:rbga(0,0,0,90);}");
    m_pTabWidegt->setStyleSheet(tab_stylesheet);


    //设置m_pPictureListWidget窗口大小位置
    m_pPictureListWidget->setStyleSheet("background-color:rgba(0,0,0,90)");
    //设置m_pPictureListWidget中的单元项的图片大小
    m_pPictureListWidget->setIconSize(QSize(120, 120));
    m_pPictureListWidget->setResizeMode(QListView::Adjust);
    //设置QListWidget的显示模式
    m_pPictureListWidget->setFlow(QListView::LeftToRight);
    m_pPictureListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pPictureListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pPictureListWidget->setViewMode(QListView::IconMode);
    m_pPictureListWidget->setFrameShape(QFrame::NoFrame);
    //设置QListWidget中的单元项不可被拖动
    m_pPictureListWidget->setMovement(QListView::Static);
    //设置QListWidget中的单元项的间距
    m_pPictureListWidget->setSpacing(20);
    //依次创建11个单元项
    for(int nIndex = 0;nIndex<15;nIndex++)
    {
        //获得图片路径
        QString strPath=QString(":/providedPosts/res/%1.JPG").arg(nIndex+1);
        //生成图像objPixmap
        QPixmap objPixmap(strPath);
        //生成QListWidgetItem对象(注意：其Icon图像进行了伸缩[96*96])---scaled函数
        QListWidgetItem *pItem = new QListWidgetItem(QPixmap(objPixmap.scaled(QSize(120,120))),
                                                     strPath);
        //设置单元项的宽度和高度
        pItem->setSizeHint(QSize(120,100));
        m_pPictureListWidget->insertItem(nIndex, pItem);

        m_index=15;//有了15张图

    }

    //设置m_pLyricsListWidget窗口的样式
    m_pLyricsListWidget->setStyleSheet("font:20px \"幼圆\";");
    m_pLyricsListWidget->setViewMode(QListView::ListMode);
    m_pLyricsListWidget->setFrameShape(QFrame::NoFrame);
    m_pLyricsListWidget->setFlow(QListView::TopToBottom);
    m_pLyricsListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pLyricsListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pLyricsListWidget->setMovement(QListView::Static);
    m_pLyricsListWidget->setResizeMode(QListView::Adjust);
    m_pLyricsListWidget->setGridSize(QSize(600,40));
    m_pLyricsListWidget->setStyleSheet(tr("QListWidget::Item{width: 600px;height:40px;color:white;font:'幼圆' 20px;background-color:rgba(0,0,0,90);}"));

    for(int nIndex = 0;nIndex<m_LyricList.length();nIndex++)
    {
        QListWidgetItem *pItem = new QListWidgetItem(m_LyricList[nIndex]);
        pItem->setFlags(Qt::NoItemFlags);
        pItem->setFlags(Qt::ItemIsEnabled);
        pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        pItem->setFont(QFont("幼圆",15));
        m_pLyricsListWidget->insertItem(nIndex, pItem);
    }


    //设置m_LyricLabel样式
    m_LyricLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_LyricLabel->setStyleSheet(tr("color:white;"));
    m_LyricLabel->setFont(QFont("Segoe Print",30));

    //设置m_pButtonWidget窗口的样式
    m_pbtnAddPicture->setText(tr("背景自定义 "));
    m_pbtnAddPicture->setStyleSheet("background-color:rgba(0,0,0,10);color:white");
    m_pbtnAddPicture->setFont(QFont("幼圆",10));
    m_pbtnChangeFont->setText(tr("歌词自定义 "));
    m_pbtnChangeFont->setStyleSheet("background-color:rgba(0,0,0,10);color:white");
    m_pbtnChangeFont->setFont(QFont("幼圆",10));
    m_pbtnSave->setText(tr("保存 "));
    m_pbtnSave->setStyleSheet("background-color:rgba(0,0,0,10);color:white");
    m_pbtnSave->setFont(QFont("幼圆",10));
    m_pbtnClose->setText(tr("关闭"));
    m_pbtnClose->setStyleSheet("background-color:rgba(0,0,0,10);color:white");
    m_pbtnClose->setFont(QFont("幼圆",10));
    QHBoxLayout *btnLayout=new QHBoxLayout;
    btnLayout->addSpacing(35);
    btnLayout->addWidget(m_pbtnAddPicture);
    btnLayout->addSpacing(35);
    btnLayout->addWidget(m_pbtnChangeFont);
    btnLayout->addSpacing(35);
    btnLayout->addWidget(m_pbtnSave);
    btnLayout->addSpacing(35);
    btnLayout->addWidget(m_pbtnClose);
    btnLayout->addSpacing(35);
    m_pButtonWidget->setLayout(btnLayout);
    m_pButtonWidget->setStyleSheet("background-color:rgba(0,0,0,90);");

    connect(m_pPictureListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemClicked1(int)));
    connect(m_pLyricsListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemClicked2(int)));
    connect(m_pbtnAddPicture,SIGNAL(clicked()),
            this,SLOT(on_btnAddPicture_clicked()));
    connect(m_pbtnChangeFont,SIGNAL(clicked()),
            this,SLOT(on_btnChangeFont_clicked()));
    connect(m_pbtnSave,SIGNAL(clicked()),
            this,SLOT(on_btnSave_clicked()));
    connect(m_pbtnClose,SIGNAL(clicked()),
            this,SLOT(close()));
}

void LyricsPost::analyzeLyrics(QString song_id)
{
    QString path=QDir::currentPath()+"/Lyrics/"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    QString result=t;
//    qDebug()<<result;
    file.close();

    //处理读取的歌词转化成好用的版本
    m_LyricList=result.split("\n");
    m_LyricList[0]=m_LyricList[0].section("[",-1,-1);
    m_LyricList[0]="["+m_LyricList[0];
    m_LyricList.pop_back();

    for(int i=0;i<m_LyricList.length();i++)
    {
        m_LyricList[i]=m_LyricList[i].section("]",-1,-1);
    }
}

void LyricsPost::on_itemClicked1(int currow)
{
    QPalette palette = m_pPictureWidget->palette();
    QListWidgetItem *pItem = m_pPictureListWidget->item(currow);
    QString path = pItem->text();
    //qDebug()<<path;
    palette.setBrush(QPalette::Window,QBrush(QPixmap(path).scaled(m_pPictureWidget->size(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation)));// 使用平滑的缩放方式
    m_pPictureWidget->setPalette(palette);// 给widget加上背景图
}

void LyricsPost::on_itemClicked2(int currow)
{
    QString lyricChosed=m_LyricList[currow];
    m_LyricLabel->setText(lyricChosed);
}

void LyricsPost::on_shareclicked(QString song_id)
{
    m_Songid=song_id;
}

void LyricsPost::on_btnAddPicture_clicked()
{
    QString curPath = QDir::homePath(); //获取用户目录
    //限制打开文件的类型
    QString dlgTitle = "选择图片(jpg/jpeg/png)";
    QString filter = "图片文件(*.jpg *.jpeg *.png);;jpg文件(*.jpg);;jpeg文件(*.jpeg);;png文件(*.png)";
    QStringList fileList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);

    if (fileList.count() < 1)
    {
        return;
     }
    for(int i=0 ; i < fileList.count() ; i++)
    {
        QString file = fileList.at(i);
        QFileInfo fileInfo(file);
        //qDebug()<<fileInfo.filePath();
        QListWidgetItem *pItem = new QListWidgetItem(QPixmap(fileInfo.filePath()).scaled(QSize(120,120)),
                                                     fileInfo.filePath());
        //设置单元项的宽度和高度
        pItem->setSizeHint(QSize(120,100));
        m_pPictureListWidget->insertItem(m_index, pItem);
        m_index++;
    }
}

void LyricsPost::on_btnChangeFont_clicked()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,this);
    if(ok)
    {
        m_LyricLabel->setFont(font);
    }
    else
    {
        //qDebug()<<"没有选择字体";
    }
}

void LyricsPost::on_btnSave_clicked()
{
    QRect rect=m_pPictureWidget->geometry();
    QPixmap ptosave=m_pPictureWidget->grab(QRect(0,0,600,600));
    QString filepath;
    QString saveDirectory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,
                                                                                  tr("选择文件的保存路径"),
                                                                                  QDir::currentPath()));

    if(!saveDirectory.isEmpty())
    {
        filepath = saveDirectory+"\\歌词海报"+m_Songid+".png";
    }
    else
    {
         ErrorWindow *pathFail = new ErrorWindow("请输入正确的文件路径");
         pathFail->show();
         pathFail->showInstantly();
    }

    if(!ptosave.save(filepath))
    {
        ErrorWindow *saveFail = new ErrorWindow("图片保存失败");
        saveFail->show();
        saveFail->showInstantly();
        this->close();
    }
    else
    {
        InformationWindow *saveSuccess = new InformationWindow("图片保存成功");
        saveSuccess->show();
        saveSuccess->showInstantly();
        this->close();
    }
}
