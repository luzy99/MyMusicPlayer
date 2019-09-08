#include "login.h"
#include<QPicture>
#include"signup.h"
#include<QDebug>
#include<errorwindow.h>
UserLogin::UserLogin(QWidget *parent)
    :QDialog (parent)
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("mytest");
    db.setUserName("root");
    db.setPassword("123456");
    if(db.open()){}


    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.8);
    setFixedSize(500,600);
    setWindowTitle("登陆窗口");
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background,QColor(25,25,25));
    setAutoFillBackground(false);
    setPalette(pal);
    setWindowIcon(QIcon(":/icon/res/user (2).png"));
    initWindow();
    //initMytitle();
    connect(signUpBtn,SIGNAL(clicked()),
            this,SLOT(on_signup_clicked()));
    connect(loginBtn, SIGNAL(clicked()), this, SLOT(on_login_clicked()));

    connect(show_pwd,SIGNAL(clicked()),
                this,SLOT(on_eye_clicked()));
    connect(m_id, SIGNAL(editingFinished()),this,SLOT(on_editingFinished()));
}

UserLogin::~UserLogin()
{

}

void UserLogin::on_signup_clicked()
{
    signup sig;
    connect(&sig,SIGNAL(returnValues(QString,QString)),
            this,SLOT(displayReturnValues(QString,QString)));
    sig.exec();
}

void UserLogin::on_login_clicked()
{
    QPalette pl;
    pl.setColor(QPalette::WindowText,QColor(95,131,210));

    if(m_id->text()==QString(""))
    {
        errorId->setPalette(pl);
        errorId->setText("该账号不存在");
    }
    else
    {
        if(m_pwd->text() != QString(""))
        {
            QSqlQuery query(db);
            query.exec(QString("select password from userinfo where userId = '%1' ;").arg(m_id->text()));
            query.next();
            if(query.value(0).toString() == m_pwd->text())
                this->done(1);
        }
        else
        {
            errorPwd->setPalette(pl);
            errorPwd->setText("密码错误");
        }

    }
}

void UserLogin::on_editingFinished()
{
    QSqlQuery query(db);
    query.exec(QString("select userImagePath from userinfo where userId = '%1' ;").arg(m_id->text()));
    query.next();
    QString imageDir = query.value(0).toString();
    pic->setPixmap(QIcon(imageDir).pixmap(this->width()/4,this->height()/4));
    pic->setFixedSize(this->width()/4,this->height()/4);
}

void UserLogin::displayReturnValues(QString id, QString pwd)
{
    m_id->setText(id);
    m_pwd->setText(pwd);
}

void UserLogin::on_eye_clicked()
{
    if(isShown)
    {
        m_pwd->setEchoMode(QLineEdit::Password);
        show_pwd->setIcon(QIcon(":/icon/res/eye.png"));
        isShown=false;
        show_pwd->setToolTip("显示密码");
    }
    else{
        m_pwd->setEchoMode(QLineEdit::Normal);
        show_pwd->setIcon(QIcon(":/icon/res/eye (1).png"));
        isShown=true;
        show_pwd->setToolTip("隐藏密码");
    }
}

void UserLogin::onClicked()
{
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());

    QWidget *pWindow = this->window(); //获得标题栏所在的窗口
    //判断发送信号的对象使哪个按钮
    if (pButton == minimizeBtn)
    {
        pWindow->showMinimized(); //窗口最小化显示
    }
    else if(pButton == closeBtn)
    {
        pWindow->close(); //窗口关闭
    }
}

void UserLogin::initTitleBar()
{
    //初始化label和btn
    icon_lb=new QLabel;
    title_lb=new QLabel;
    minimizeBtn=new QPushButton;
    closeBtn=new QPushButton;
    //初始化图标和文字
    icon_lb->setFixedSize(20,20);
    icon_lb->setScaledContents(true);
    icon_lb->setPixmap(QPixmap(":/icon/res/icon (2).png"));

    title_lb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    title_lb->setText("          欢迎登陆");
    QPalette pl;
    pl.setColor(QPalette::WindowText,QColor(95,131,210));
    title_lb->setPalette(pl);
    title_lb->setFont(QFont("微软雅黑", 12));
    title_lb->setAlignment(Qt::AlignCenter);
    title_lb->setFixedWidth(160);
    //设置按钮的固定大小、图片、取消边框
    minimizeBtn->setIconSize(QSize(20,20));
    minimizeBtn->setIcon(QIcon(":/icon/res/minscreen.png"));
    minimizeBtn->setToolTip("最小化");
    minimizeBtn->setFlat(true);
    minimizeBtn->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    closeBtn->setIconSize(QSize(20,20));
    closeBtn->setIcon(QIcon(":/icon/res/close.png"));
    closeBtn->setFlat(true);
    closeBtn->setToolTip("关闭窗口");
    closeBtn->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    titleQhb=new QHBoxLayout;
    titleQhb->addWidget(icon_lb);
    titleQhb->addSpacing(100);
    titleQhb->addWidget(title_lb);
    titleQhb->addSpacing(100);
    titleQhb->addWidget(minimizeBtn);
    titleQhb->addWidget(closeBtn);

    //连接信号和槽
    connect(minimizeBtn, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(closeBtn, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void UserLogin::initWindow()
{
    initTitleBar();
    isShown=false;
    QMovie *background;//登陆背景动画
    QLabel *pBack;
    //背景动态图
    pBack=new QLabel;
    background=new QMovie;
    background->setFileName(":/icon/res/Changed.gif");
    pBack->setMovie(background);
    background->start();
    pBack->move(0,0);
    //账号输入
    accounts=new QComboBox;
    accounts->setEditable(true);
    accounts->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    m_id=accounts->lineEdit();
    m_id->setPlaceholderText(QStringLiteral("请输入账号"));
    m_pwd=new QLineEdit;
    m_pwd->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pwd->setEchoMode(QLineEdit::Password);
    m_pwd->setStyleSheet( "border:2px groove gray;border-radius:10px;padding:2px 4px");
    QHBoxLayout *h1=new QHBoxLayout;

    QHBoxLayout *h2=new QHBoxLayout;

    //显示密码的按钮
    show_pwd=new QPushButton;
    errorPwd=new QLabel;
    //show_pwd=new QPushButton;
    show_pwd->setObjectName("显示密码");
    show_pwd->setFixedSize(QSize(16,16));
    show_pwd->setFlat(true);
    show_pwd->setCursor(QCursor(Qt::PointingHandCursor));
    show_pwd->setIcon(QIcon(":/icon/res/eye.png"));

    //用于提示id不存在
    errorId=new QLabel;
    errorId->setFixedWidth(width()/4);
    //提示用户名与密码不符
    errorPwd=new QLabel;
    errorPwd->setFixedWidth(width()/4);




    //注册按钮
    signUpBtn=new QPushButton;
    signUpBtn->setToolTip("注册账户");
    signUpBtn->setStyleSheet("border:none");
    signUpBtn->setIcon(QIcon(":/icon/res/register.png"));
    signUpBtn->setFixedSize(QSize(m_id->height()+10,m_id->height()+10));
    signUpBtn->setCursor(QCursor(Qt::PointingHandCursor));

    //账号框布局
    QLabel *user_icon =new QLabel;
    user_icon->setPixmap(QIcon(":/icon/res/user (2).png").pixmap(m_id->height(),m_id->height()));
    user_icon->setFixedSize(m_id->height(),m_id->height());
    user_icon->setToolTip("账号");
    h1->addWidget(user_icon);
    h1->addWidget(accounts);
    h1->addWidget(signUpBtn);
    h1->addWidget(errorId);

    //密码框布局
    QLabel * pwd_icon=new QLabel;
    pwd_icon->setPixmap(QIcon(":/icon/res/lock.png").pixmap(m_id->height(),m_id->height()));
    pwd_icon->setFixedSize(m_id->height(),m_id->height());
    pwd_icon->setToolTip("密码");
    h2->addWidget(pwd_icon);
    h2->addWidget(m_pwd);
    h2->addWidget(show_pwd);
    h2->addWidget(errorPwd);


    pic=new QLabel;
    pic->setPixmap(QIcon(":/icon/res/icon (2).png").pixmap(this->width()/4,this->height()/4));
    pic->setFixedSize(QSize(width()/4,height()/4));
    QVBoxLayout *QVB=new QVBoxLayout;
    QVB->setContentsMargins(0,0,0,0);
    QVB->addLayout(h1);
    QVB->addLayout(h2);
    QHBoxLayout *QHB=new QHBoxLayout;
    QHB->addWidget(pic);
    QHB->addLayout(QVB);

    loginBtn=new QPushButton;
    loginBtn->setText("登陆");
    QString BtnStyle;
    BtnStyle="QPushButton{background-color:rgba(95,131,210,75%);"
            "color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
            "QPushButton:hover{background-color:rgb(85, 170, 255); color: black;}"  // 鼠标停放时的色彩
            "QPushButton:pressed{background-color:white; border-style: inset; }"; // 鼠标按下的色彩
    loginBtn->setStyleSheet(BtnStyle);
    loginBtn->setFixedSize(80,30);

    QHBoxLayout *QHBB=new QHBoxLayout;
    QHBB->setContentsMargins(0,0,0,0);
    QHBB->addSpacing(180);
    QHBB->addWidget(loginBtn);
    QHBB->addSpacing(180);

    QVBoxLayout *QVBB=new QVBoxLayout;
    //添加自定义标题栏
    QVBB->addLayout(titleQhb);
    //添加背景动图
    QVBB->addWidget(pBack);

    //添加账号密码布局
    QVBB->addLayout(QHB);

    QVBB->addSpacing(15);
    //登陆按钮
    QVBB->addLayout(QHBB);

    QVBB->addSpacing(15);
    setLayout(QVBB);

}

QLineEdit *UserLogin::id() const
{
    return m_id;
}



