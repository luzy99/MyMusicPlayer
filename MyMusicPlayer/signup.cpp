#include "signup.h"
#include "errorwindow.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QDebug>
#include<QSqlQuery>
#include<QMovie>
#include <QDir>
#include <QFileDialog>
#include <QBitmap>
#include <QPainter>

signup::signup(QWidget *parent)
    :QDialog (parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(0.9);
    this->setFixedSize(500,600);
    this->setWindowTitle("注册窗口");
    this->setStyleSheet("border: 1px solid rgb(25,25,25);");
    setWindowIcon(QIcon(":/icon/res/user (2).png"));

    //绘制圆角窗口
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),2,2);
    setMask(bmp);

    initWindows();

    connect(okBtn,SIGNAL(clicked()),
            this,SLOT(on_return_clicked()));
    connect(cancelBtn,SIGNAL(clicked()),
            this,SLOT(on_cancel_clicked()));
    connect(imageBtn, SIGNAL(clicked()),
            this, SLOT(on_imageBtn_clicked()));
}

signup::~signup()
{

}

void signup::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}
void signup::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

//注册成功的槽函数
void signup::on_return_clicked()
{
    //获取用户的输入信息
    QString pwd1 = pwd1LineEdit->text();
    QString pwd2 = pwd2LineEdit->text();
    QString name=nameLineEdit->text();

    //判断可能的注册错误并做出提示
    if(name == QString(""))
    {
        ErrorWindow *emptyError = new ErrorWindow("昵称不能为空");
        emptyError->show();
        emptyError->showInstantly();
        return;
    }
    else if(pwd1.length() < 6)
    {

            ErrorWindow *pswError = new ErrorWindow("密码少于6位");
            pswError->show();
            pswError->showInstantly();
            return;
     }
    else if(pwd1 != pwd2)
     {
            ErrorWindow *pswError = new ErrorWindow("密码不一致");
            pswError->show();
            pswError->showInstantly();
            return;
      }
    else
    {
    }

    //注册成功,在数据库里为新用户创建歌单
    QSqlQuery query;
    QString createSongList1 = "create table %1 (songName varchar(255) ,"
                                          "songUrl varchar(255) primary key, likeOrNot int(1), "
                                          "artist varchar(255), album varchar(255), cover_image varchar(255),num int(9));";
    QString createSongList2 = "create table %1 (songName varchar(255) ,"
                                          "songUrl varchar(255) primary key, likeOrNot int(1), "
                                          "artist varchar(255), album varchar(255), cover_image varchar(255),num int(9));";
     //注册信息添加入userinfo表
     query.exec(QString("insert into userinfo values('%1', '%2' , '%3');").arg(strId, pwd1, name));
     //用户图像存入指定目录
     QPixmap headImage;
     headImage.load(userImagedir);
     headImage.save(QDir::currentPath()+"/userHeads/"+strId+".png");
     query.exec(createSongList1.arg(strId+"_我喜欢的音乐"));
     query.exec(createSongList2.arg(strId+"_播放历史"));
     emit returnValues(strId,pwd1);
     this->done(1);
}

//点击取消的槽函数
void signup::on_cancel_clicked()
{
    this->done(0);
}

void signup::on_imageBtn_clicked()
{
    QString curPath = QDir::homePath(); //获取用户目录
    //限制打开文件的类型
    QString dlgTitle = "选择图片文件";
    QString filter = "图片文件(*.jpg *.png *.bmp);;jpg文件(*.jpg);;png文件(*.png);;bmp文件(*.bmp);;所有文件(*.*)";
    QString file = QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    QFileInfo fileInfo(file);
    if(!fileInfo.isReadable())
    {
        ErrorWindow *imageWindow = new ErrorWindow("头像图片打开失败");
        imageWindow->show();
        imageWindow->showInstantly();
        return;
    }

    QPixmap srcImage(file);
    QSize size = imageBtn->size();
    QPixmap headImage = srcImage.scaled(size,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    imageBtn->setIcon(QIcon(headImage));
    userImagedir = file;
}

void signup::initWindows()
{
    QSqlQuery query;
    query.exec(QString("select count(*) from userinfo"));
    query.next();
    int num = query.value(0).toInt();
    userImagedir = ":/icon/res/default_cover.png"; //设置默认头像

    //初始化背景图
    QMovie *background = new QMovie;//登陆背景动画
    QLabel *pBack = new QLabel(this);
    pBack->setFixedSize(500, 320);
    //QHBoxLayout *backMovieLayout =new QHBoxLayout;
    background->setFileName(":/icon/res/Changed.gif");
    pBack->setMovie(background);
    background->start();
    pBack->move(0, 0);

    int space=50;
    pl.setColor(QPalette::WindowText,QColor(95,131,210));
    //欢迎注册   

    tipWidget=new QWidget(this);
    welWidget= new QWidget(this);
    welWidget->setGeometry(0,0,500,40);
    welWidget->setStyleSheet("border:none");
    tipWidget->setGeometry(0,280,500,40);
    tipWidget->setStyleSheet("border:none");

    welcome_lb=new QLabel(welWidget);
    welcome_lb->setText(QString("欢迎注册Rainbow音乐播放器"));
    welcome_lb->setFont(QFont("微软雅黑", 11));
    welcome_lb->setStyleSheet("border:none");
    welcome_lb->setAlignment(Qt::AlignCenter);
    welcome_lb->setPalette(pl);

    info_lb=new QLabel(tipWidget);
    strId=QString("u")+QString::number(1000000001+num);
    info_lb->setText(QString("您的账号为:%1").arg(strId));
    info_lb->setFont(QFont("微软雅黑", 11));
    info_lb->setStyleSheet("border:none");
    info_lb->setAlignment(Qt::AlignCenter);
    info_lb->setPalette(pl);

    QHBoxLayout *qhb1=new QHBoxLayout;
    qhb1->addSpacing(space);
    qhb1->addWidget(welcome_lb);
    qhb1->addSpacing(space);
    welWidget->setLayout(qhb1);
    QHBoxLayout *qhb6=new QHBoxLayout;
    qhb6->addSpacing(space);
    qhb6->addWidget(info_lb);
    qhb6->addSpacing(space);
    welWidget->setLayout(qhb1);
    tipWidget->setLayout(qhb6);

    infoWidget=new QWidget(this);
    infoWidget->setGeometry(0,320,500,300);
    infoWidget->setStyleSheet("background: rgb(25,25,25);");
    QLabel *name_lb=new QLabel;
    name_lb->setText("请输入昵称");
    name_lb->setPalette(pl);

    //输入昵称的框
    nameLineEdit=new QLineEdit;
    QLabel *pwd_lb=new QLabel;
    pwd_lb->setText("请输入密码");
    pwd_lb->setPalette(pl);
    QLabel *pwd_lb2=new QLabel;
    pwd_lb2->setText("请确认密码");
    pwd_lb2->setPalette(pl);
    pwd1LineEdit=new QLineEdit;
    pwd2LineEdit=new QLineEdit;

    nameLineEdit->setStyleSheet( "QLineEdit{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(100,100,100,75);border-bottom-color:white;border-bottom-width:10px;}"
                                 "QLineEdit::Hover{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(165,150,150,75);border-bottom-color:white;border-bottom-width:10px;}");
    pwd1LineEdit->setStyleSheet( "QLineEdit{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(100,100,100,75);border-bottom-color:white;border-bottom-width:10px;}"
                                 "QLineEdit::Hover{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(165,150,150,75);border-bottom-color:white;border-bottom-width:10px;}");
    pwd2LineEdit->setStyleSheet( "QLineEdit{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(100,100,100,75);border-bottom-color:white;border-bottom-width:10px;}"
                                 "QLineEdit::Hover{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                                 "background-color:rgba(165,150,150,75);border-bottom-color:white;border-bottom-width:10px;}");
    nameLineEdit->setPlaceholderText("输入字母、数字、下划线和汉字");
    pwd1LineEdit->setPlaceholderText("至少6位字母或者数字或者组合");
    pwd2LineEdit->setPlaceholderText("请再次输入密码");
    //设定密码为隐藏状态
    pwd1LineEdit->setEchoMode(QLineEdit::Password);
    pwd2LineEdit->setEchoMode(QLineEdit::Password);
    //按钮
    okBtn=new QPushButton;
    okBtn->setText("确定");
    cancelBtn=new QPushButton;
    cancelBtn->setText("取消");

    imageBtn = new QPushButton;

    okBtn->setFixedSize(100,40);
    cancelBtn->setFixedSize(100,40);
    imageBtn->setFixedSize(140,140);

    QString BtnStyle;
    BtnStyle="QPushButton{background-color:rgba(95,131,210,75%);"
             "color: white; border-radius: 2px;}"
             "QPushButton:hover{background-color:rgba(82,113,180,75%);"
             "color: white; border-radius: 2px;}";
    okBtn->setStyleSheet(BtnStyle);
    cancelBtn->setStyleSheet(BtnStyle);
    imageBtn->setFlat(true);
    imageBtn->setStyleSheet("border: 2px solid rgb(190,190,190);border-radius: 5px;");

    QPixmap srcImage1(userImagedir);
    QSize size = imageBtn->size();
    QPixmap userImage = srcImage1.scaled(size,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    imageBtn->setIcon(QIcon(userImage));
    imageBtn->setIconSize(QSize(140, 140));

    QHBoxLayout *qhb2=new QHBoxLayout;
    QHBoxLayout *qhb3=new QHBoxLayout;
    QHBoxLayout *qhb4=new QHBoxLayout;
    QHBoxLayout *qhb5=new QHBoxLayout;

    qhb2->addSpacing(space);
    qhb2->addWidget(name_lb);
    qhb2->addWidget(nameLineEdit);

    qhb3->addSpacing(space);
    qhb3->addWidget(pwd_lb);
    qhb3->addWidget(pwd1LineEdit);

    qhb4->addSpacing(space);
    qhb4->addWidget(pwd_lb2);
    qhb4->addWidget(pwd2LineEdit);

    qhb5->addSpacing(space);
    qhb5->addWidget(okBtn);
    qhb5->addSpacing(100);
    qhb5->addWidget(cancelBtn);
    qhb5->addSpacing(space);
    QVBoxLayout *qvb=new QVBoxLayout;

    QVBoxLayout *VBoxForThree =new QVBoxLayout;
    VBoxForThree->setContentsMargins(0,0,0,0);
    VBoxForThree->addLayout(qhb2);
    VBoxForThree->addSpacing(10);
    VBoxForThree->addLayout(qhb3);
    VBoxForThree->addSpacing(10);
    VBoxForThree->addLayout(qhb4);

    QHBoxLayout *HBoxForImg = new QHBoxLayout;
    HBoxForImg->setSpacing(0);
    HBoxForImg->setContentsMargins(20,0,10,0);
    HBoxForImg->addWidget(imageBtn);
    HBoxForImg->addLayout(VBoxForThree);

    qvb->addLayout(HBoxForImg);
    qvb->addSpacing(10);
    qvb->addLayout(qhb5);
    qvb->addSpacing(10);
    infoWidget->setLayout(qvb);
    welWidget->raise();
    tipWidget->raise();
}




