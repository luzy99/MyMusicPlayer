#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QMouseEvent>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置窗口大小
    this->setFixedSize(600,1000);

    //初始化窗口组件
    m_pPictureWidget =new QWidget(this);
    m_pTabWidegt=new QTabWidget(this);
    m_pPictureListWidget=new QListWidget(this);
    m_pLyricsListWidget=new QListWidget(this);
    m_LyricLabel=new QLabel(m_pPictureWidget);
    m_pButtonWidget=new QWidget(this);
    m_pbtnAddPicture=new QPushButton;
    m_pbtnChangeFont=new QPushButton;
    m_pbtnSave=new QPushButton;
    m_index=0;

    //设置事件过滤器
    m_LyricLabel->installEventFilter(this);
    //m_LyricLabel->setText("hhh,test");
    analyzeLyrics("1366903039");

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
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/test/C:/Users/Lenovo/Desktop/背景图片/1.JPG").scaled(m_pPictureWidget->size(),
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
    for(int nIndex = 0;nIndex<16;nIndex++)
    {
        //获得图片路径
        QString strPath=QString(":/test/C:/Users/Lenovo/Desktop/背景图片/%1.JPG").arg(nIndex+1);
        //生成图像objPixmap
        QPixmap objPixmap(strPath);
        //生成QListWidgetItem对象(注意：其Icon图像进行了伸缩[96*96])---scaled函数
        QListWidgetItem *pItem = new QListWidgetItem(QPixmap(objPixmap.scaled(QSize(120,120))),"animal tiger pig");
        //设置单元项的宽度和高度
        pItem->setSizeHint(QSize(120,100));
        m_pPictureListWidget->insertItem(nIndex, pItem);

        m_index=16;//有了16张图

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
    QHBoxLayout *btnLayout=new QHBoxLayout;
    btnLayout->addSpacing(60);
    btnLayout->addWidget(m_pbtnAddPicture);
    btnLayout->addSpacing(60);
    btnLayout->addWidget(m_pbtnChangeFont);
    btnLayout->addSpacing(60);
    btnLayout->addWidget(m_pbtnSave);
    btnLayout->addSpacing(60);
    m_pButtonWidget->setLayout(btnLayout);
    m_pButtonWidget->setStyleSheet("background-color:rgba(0,0,0,90);");

    connect(m_pPictureListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemClicked1(int)));
    connect(m_pLyricsListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(on_itemClicked2(int)));
    connect(m_pbtnAddPicture,SIGNAL(clicked()),this,SLOT(on_btnAddPicture_clicked()));
    connect(m_pbtnChangeFont,SIGNAL(clicked()),this,SLOT(on_btnChangeFont_clicked()));
    connect(m_pbtnSave,SIGNAL(clicked()),this,SLOT(on_btnSave_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::analyzeLyrics(QString song_id)
{
    QString path="D:/Git/clone/Lyric/Lyric"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    QString result=t;
    //qDebug()<<result;
    file.close();

    //处理读取的歌词转化成好用的版本
    m_LyricList=result.split("\n");
    m_LyricList[0]=m_LyricList[0].section("[",-1,-1);
    m_LyricList[0]="["+m_LyricList[0];
    m_LyricList.pop_back();
    //qDebug()<<list;

    for(int i=0;i<m_LyricList.length();i++)
    {
        m_LyricList[i]=m_LyricList[i].section("]",-1,-1);
    }
}

void MainWindow::on_itemClicked1(int currow)
{
    QPalette palette = m_pPictureWidget->palette();
    QString path=":/test/C:/Users/Lenovo/Desktop/背景图片/%1.JPG";
    QString index=QString::number(currow+1);
    path=path.arg(index);
    palette.setBrush(QPalette::Window,QBrush(QPixmap(path).scaled(m_pPictureWidget->size(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation)));// 使用平滑的缩放方式
    m_pPictureWidget->setPalette(palette);// 给widget加上背景图
}

void MainWindow::on_itemClicked2(int currow)
{
    QString lyricChosed=m_LyricList[currow];
    m_LyricLabel->setText(lyricChosed);
}



void MainWindow::on_shareclicked(QString song_id)
{
    m_Songid=song_id;
}

void MainWindow::on_btnAddPicture_clicked()
{
    //姐妹留给你写了，读取图片
    //m_pPictureListWidget->insertItem(m_index, pItem);
    m_index++;

}

void MainWindow::on_btnChangeFont_clicked()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,this);
    if(ok)
    {
        m_LyricLabel->setFont(font);
    }
    else
    {
        qDebug()<<"没有选择字体";
    }
}

void MainWindow::on_btnSave_clicked()
{
    //截图，保存靠你了姐妹
    QRect rect=m_pPictureWidget->geometry();
    QPixmap ptosave=m_pPictureWidget->grab(QRect(0,0,600,600));
    QString filepath="C:\\Users\\Lenovo\\Desktop\\背景图片\\haha.png";
    if(!ptosave.save(filepath,"png"))

        {

            qDebug()<<"save widget screen failed"<<endl;

        }
}

//bool MainWindow::eventFilter(QObject *obj, QEvent *event)
//{
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        if(obj==m_LyricLabel)
//        {
//            QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
//            m_mouseStartPoint = mouseevent->globalPos();
//            return true;
//        }

//    }


//    if(event->type() == QEvent::MouseMove)
//    {
//        if(obj==m_LyricLabel)
//        {
//            QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
//            QPoint offset=mouseevent->globalPos()-m_mouseStartPoint;
//            m_LyricLabel->move(offset);
//            return true;
//        }
//    }

//}
