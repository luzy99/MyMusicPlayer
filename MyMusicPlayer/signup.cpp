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
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setFixedSize(500,600);
    setWindowTitle("注册窗口");
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background,QColor(25,25,25));
    setAutoFillBackground(false);
    setPalette(pal);
    setWindowIcon(QIcon(":/icon/res/user (2).png"));

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

QPixmap signup::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
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
    headImage = PixmapToRound(headImage,size.height()/2);
    imageBtn->setIcon(QIcon(headImage));
    userImagedir = file;
}

void signup::initWindows()
{
    QSqlQuery query;
    query.exec(QString("select count(*) from userinfo"));
    query.next();
    int num = query.value(0).toInt();
    qDebug()<<num;
    userImagedir = ":/icon/res/defaultUser.png"; //设置默认头像
    QMovie *background;//登陆背景动画
    QLabel *pBack;
    //背景动态图
    pBack=new QLabel;
    //QHBoxLayout *backMovieLayout =new QHBoxLayout;
    background=new QMovie;
    background->setFileName(":/icon/res/Changed.gif");
    background->setScaledSize(QSize(400, 250));
    pBack->setMovie(background);
    pBack->setAlignment(Qt::AlignCenter);
    background->start();
    pBack->move(0, 20);

    int space=50;
    //设置提示色为红色
    m_pal.setColor(QPalette::WindowText,Qt::red);
    pl.setColor(QPalette::WindowText,QColor(95,131,210));
    //欢迎注册   

    welcome_lb=new QLabel;
    strId=QString("u")+QString::number(1000000001+num);
    welcome_lb->setText(QString("欢迎注册Rainbow音乐播放器\n您的账号为:%1").arg(strId));
    welcome_lb->setFont(QFont("微软雅黑", 11));
    welcome_lb->setAlignment(Qt::AlignCenter);
    welcome_lb->setPalette(pl);

    QLabel *name_lb=new QLabel;
    name_lb->setText("请输入昵称");
    name_lb->setPalette(pl);

    //输入昵称的框
    nameLineEdit=new QLineEdit;
    //
    QLabel *pwd_lb=new QLabel;
    pwd_lb->setText("请输入密码");
    pwd_lb->setPalette(pl);
    QLabel *pwd_lb2=new QLabel;
    pwd_lb2->setText(" 确认密码 ");
    pwd_lb2->setPalette(pl);
    //
    pwd1LineEdit=new QLineEdit;
    pwd2LineEdit=new QLineEdit;
    nameLineEdit->setPlaceholderText("输入字母、数字、下划线和汉字");
    pwd1LineEdit->setPlaceholderText("至少6位字母或者数字或者组合");
    pwd2LineEdit->setPlaceholderText("请再次输入密码");

    nameLineEdit->setStyleSheet( "border:2px groove gray;border-radius:10px;padding:2px 4px");
    pwd1LineEdit->setStyleSheet( "border:2px groove gray;border-radius:10px;padding:2px 4px");
    pwd2LineEdit->setStyleSheet( "border:2px groove gray;border-radius:10px;padding:2px 4px");
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
    imageBtn->setFixedSize(80,80);

    QString BtnStyle;
    BtnStyle="QPushButton{background-color:rgba(95,131,210,75%);"
            "color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
            "QPushButton:hover{background-color:rgb(85, 170, 255); color: black;}"  // 鼠标停放时的色彩
            "QPushButton:pressed{background-color:white; border-style: inset; }"; // 鼠标按下的色彩
    okBtn->setStyleSheet(BtnStyle);
    cancelBtn->setStyleSheet(BtnStyle);
    imageBtn->setFlat(true);
    QIcon userImage(userImagedir);
    imageBtn->setIcon(userImage);
    imageBtn->setIconSize(QSize(80, 80));

    //
    QHBoxLayout *qhb1=new QHBoxLayout;
    QHBoxLayout *qhb2=new QHBoxLayout;
    QHBoxLayout *qhb3=new QHBoxLayout;
    QHBoxLayout *qhb4=new QHBoxLayout;
    QHBoxLayout *qhb5=new QHBoxLayout;

    //
    qhb1->addSpacing(space);
    qhb1->addWidget(welcome_lb);
    qhb1->addSpacing(space);
    //
    qhb2->addSpacing(space);
    qhb2->addWidget(name_lb);
    qhb2->addWidget(nameLineEdit);

    //
    qhb3->addSpacing(space);
    qhb3->addWidget(pwd_lb);
    qhb3->addWidget(pwd1LineEdit);

    //
    qhb4->addSpacing(space);
    qhb4->addWidget(pwd_lb2);
    qhb4->addWidget(pwd2LineEdit);

    //
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
    HBoxForImg->addWidget(imageBtn);
    HBoxForImg->addLayout(VBoxForThree);
    qvb->addWidget(pBack);
    qvb->addLayout(qhb1);
    qvb->addLayout(HBoxForImg);
    qvb->addSpacing(10);
    qvb->addLayout(qhb5);
    qvb->addSpacing(10);
    setLayout(qvb);
}




