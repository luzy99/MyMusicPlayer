#include "login.h"
#include"signup.h"
#include "errorwindow.h"
#include "informationwindow.h"
#include <QPicture>
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QPropertyAnimation>
#include <QDir>

UserLogin::UserLogin(QWidget *parent)
    :QDialog (parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(0.9);
    this->setFixedSize(500,600);
    this->setWindowTitle("登陆窗口");
    this->setStyleSheet("border: 1px solid rgb(25,25,25);");
    this->setWindowIcon(QIcon(":/icon/res/user (2).png"));
    //initMytitle();

    //绘制圆角窗口
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),2,2);
    setMask(bmp);

    //初始化计时器
    m_timer=new QTimer;

    //初始化标题按钮
    m_ptitleBar=new QWidget(this);
    m_ptitleBar->setGeometry(0,0,500,50);
    m_ptitleBar->setStyleSheet("border:none;");
    //初始化label和btn
    icon_lb=new QLabel;
    title_lb=new QLabel;
    minimizeBtn=new QPushButton;
    closeBtn=new QPushButton;
    //初始化图标和文字
    icon_lb->setFixedSize(25,25);
    icon_lb->setScaledContents(true);
    icon_lb->setPixmap(QPixmap(":/icon/res/icon (2).png"));

    title_lb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    title_lb->setText("     欢迎登陆");
    title_lb->setStyleSheet("color: rgb(95,131,210);"
                            "border: none;");
    title_lb->setFont(QFont("微软雅黑", 12));
    title_lb->setAlignment(Qt::AlignCenter);
    title_lb->setFixedWidth(160);
    //设置按钮的固定大小、图片、取消边框
    minimizeBtn->setIconSize(QSize(20,20));
    minimizeBtn->setIcon(QIcon(":/icon/res/minscreen.png"));
    minimizeBtn->setToolTip("最小化");
    minimizeBtn->setFlat(true);
    minimizeBtn->setStyleSheet("border:none;");
    closeBtn->setIconSize(QSize(20,20));
    closeBtn->setIcon(QIcon(":/icon/res/close.png"));
    closeBtn->setFlat(true);
    closeBtn->setToolTip("关闭窗口");
    closeBtn->setStyleSheet("border:none;");
    QHBoxLayout *titleQhb=new QHBoxLayout;
    titleQhb=new QHBoxLayout;
    titleQhb->addWidget(icon_lb);
    titleQhb->addSpacing(100);
    titleQhb->addWidget(title_lb);
    titleQhb->addSpacing(100);
    titleQhb->addWidget(minimizeBtn);
    titleQhb->addWidget(closeBtn);
    m_ptitleBar->setLayout(titleQhb);

    //初始化背景动图
    m_backgif=new QLabel(this);
    m_backgif->setFixedSize(500,300);
    m_backgif->setGeometry(0,0,500,300);
    QMovie *background=new QMovie;;//登陆背景动画
    //背景动态图
    background->setFileName(":/icon/res/Changed.gif");
    m_backgif->setMovie(background);
    background->start();
    m_backgif->move(0,0);
    m_ptitleBar->raise();

    m_infoWidget=new QWidget(this);
    m_infoWidget->setGeometry(0,300,500,300);
    m_infoWidget->setStyleSheet("background-color:rgb(25,25,25);");
    //账号输入
    accounts=new QComboBox(this);
    accounts->setFixedSize(QSize(260,40));
    accounts->setEditable(true);
    accounts->setStyleSheet( "QComboBox{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                             "background-color:rgba(100,100,100,75);border-bottom-color:white;border-bottom-width:10px;}"
                             "QComboBox::Hover{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                             "background-color:rgba(165,150,150,75);border-bottom-color:white;border-bottom-width:10px;}"
                             "QComboBox::drop-down{border:none;}"
                             "QComboBox::down-arrow{image: url(:/icon/res/dropArrow.png);height:35px;width:20px;}");
    m_id=accounts->lineEdit();
    m_id->setPlaceholderText(QStringLiteral("请输入账号"));
    //注册按钮
    signUpBtn=new QPushButton;
    signUpBtn->setToolTip("注册账户");
    signUpBtn->setStyleSheet("border:none");
    signUpBtn->setIcon(QIcon(":/icon/res/register.png"));
    signUpBtn->setFixedSize(QSize(20,20));
    signUpBtn->setCursor(QCursor(Qt::PointingHandCursor));
    //账号框布局
    QLabel *user_icon =new QLabel;
    user_icon->setPixmap(QIcon(":/icon/res/user (2).png").pixmap(20,20));
    user_icon->setFixedSize(20,20);
    user_icon->setToolTip("账号");
    QHBoxLayout *h1=new QHBoxLayout;
    h1->addSpacing(80);
    h1->addWidget(user_icon);
    h1->addWidget(accounts);
    h1->addWidget(signUpBtn);
    h1->addSpacing(80);
    //m_infoWidget->setLayout(h1);

    m_pwdIcon=new QLabel;
    m_pwdIcon->setPixmap(QIcon(":/icon/res/lock.png").pixmap(20,20));
    m_pwdIcon->setFixedSize(20,20);
    m_pwdIcon->setToolTip("密码");
    m_pwd=new QLineEdit;
    m_pwd->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pwd->setEchoMode(QLineEdit::Password);
    m_pwd->setStyleSheet( "QLineEdit{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                          "background-color:rgba(100,100,100,75);border-bottom-color:white;border-bottom-width:10px;}"
                          "QLineEdit::Hover{color:white;border:0px;border-radius:2px;width:200px;height:40px;"
                          "background-color:rgba(165,150,150,75);border-bottom-color:white;border-bottom-width:10px;}");
    //显示密码的按钮
    show_pwd=new QPushButton;
    //show_pwd=new QPushButton;
    show_pwd->setObjectName("显示密码");
    show_pwd->setFixedSize(QSize(20,20));
    show_pwd->setFlat(true);
    show_pwd->setCursor(QCursor(Qt::PointingHandCursor));
    show_pwd->setIcon(QIcon(":/icon/res/eye.png"));
    show_pwd->setDefault(false);
    QLabel * pwd_icon=new QLabel;
    pwd_icon->setPixmap(QIcon(":/icon/res/lock.png").pixmap(20,20));
    pwd_icon->setFixedSize(20,20);
    pwd_icon->setToolTip("密码");
    QHBoxLayout *h2=new QHBoxLayout;
    h2->addSpacing(80);
    h2->addWidget(m_pwdIcon);
    h2->addWidget(m_pwd);
    h2->addWidget(show_pwd);
    h2->addSpacing(80);
    //m_infoWidget->setLayout(h2);

    loginBtn=new QPushButton(this);
    loginBtn->setText("登 陆");
    QString BtnStyle;
    BtnStyle="QPushButton{background-color:rgba(95,131,210,75%);"
             "color: white; border-radius: 2px;}"
             "QPushButton:hover{background-color:rgba(82,113,180,75%);"
             "color: white; border-radius: 2px;}";
    loginBtn->setFont(QFont("微软雅黑", 12,25));
    loginBtn->setStyleSheet(BtnStyle);
    loginBtn->setFixedSize(264,50);
    QHBoxLayout *h3=new QHBoxLayout;
    h3->addSpacing(107);
    h3->addWidget(loginBtn);
    h3->addSpacing(125);
    //m_infoWidget->setLayout(h3);

    QVBoxLayout *v1=new QVBoxLayout;
    v1->addSpacing(50);
    v1->addLayout(h1);
    v1->addLayout(h2);
    v1->addLayout(h3);
    m_infoWidget->setLayout(v1);

    QSize size(100, 100);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = QIcon(":/icon/res/icon (2).png").pixmap(75,75);
    image.setMask(mask);

    pic = new QLabel(this);
    pic->setGeometry(200,248,100,100);
    pic->setAlignment(Qt::AlignCenter);
    pic->setPixmap(image);
    pic->setStyleSheet("max-width:100;min-width:100;max-height:100;"
                       "min-height:100;border:4px solid rgb(200,200,200);"
                       "border-radius:52px;background-color:white;");
    pic->installEventFilter(this);

    m_addPic = new QLabel(this);
    m_addPic->setGeometry(225,273,50,50);
    m_addPic->setAlignment(Qt::AlignCenter);
    m_addPic->setPixmap(QIcon(":/icon/res/logOut.png").pixmap(50,50));
    m_addPic->setStyleSheet("max-width:50;min-width:50;max-height:50;"
                            "min-height:50;border:4px solid rgb(240,240,240);"
                            "border-radius:27px;background-color:white;");
    m_addPic->setToolTip("登出");
    m_addPic->installEventFilter(this);
    pic->raise();

    //连接信号和槽
    connect(minimizeBtn, SIGNAL(clicked(bool)),
            this, SLOT(onClicked()));
    connect(closeBtn, SIGNAL(clicked(bool)),
            this, SLOT(onClicked()));
    connect(m_timer,SIGNAL(timeout()),
            this,SLOT(on_timeout()));
    connect(signUpBtn,SIGNAL(clicked()),
            this,SLOT(on_signup_clicked()));
    connect(loginBtn, SIGNAL(clicked()),
            this, SLOT(on_login_clicked()));
    connect(show_pwd,SIGNAL(clicked()),
            this,SLOT(on_eye_clicked()));
    connect(m_id, SIGNAL(editingFinished()),
            this,SLOT(on_editingFinished()));
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

//登录按钮点击后
void UserLogin::on_login_clicked()
{
    if(m_id->text() == "")
    {
        ErrorWindow *emptyError = new ErrorWindow("请输入用户名");
        emptyError->show();
        emptyError->showInstantly();
        return;
    }
    else if(m_pwd->text()== "")
    {
        ErrorWindow *emptyError = new ErrorWindow("请输入密码");
        emptyError->show();
        emptyError->showInstantly();
        return;
    }
    else
    {
    }

    QSqlQuery query;
    query.exec(QString("select password from userinfo where userId = '%1' ;").arg(m_id->text()));
    query.next();

    if(query.value(0).toString() == m_pwd->text())
    {
        //用户信息正确,登录成功,向外抛出用户的id
        this->done(1);
        emit loginSuccess(m_id->text());
    }
    else
    {
        ErrorWindow *pswError = new ErrorWindow("用户名或密码错误");
        pswError->show();
        pswError->showInstantly();
        return;
     }
}

void UserLogin::on_editingFinished()
{
    //首先判断用户是否存在
    QSqlQuery query;
    QString judgeExist = "select password from userinfo where userId= '%1' ; ";
    query.exec(judgeExist.arg(m_id->text()));
    query.next();
    if(query.value(0).toString() == "")
    {
        //如果用户不存在,则不做操作
        return;
    }

    QString headImagePath = QDir::currentPath()+"/userHeads/"+m_id->text();
    QSize size(100, 100);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = QIcon(headImagePath).pixmap(100,100);
    image.setMask(mask);
    pic->setGeometry(200,248,100,100);
    pic->setAlignment(Qt::AlignCenter);
    pic->setPixmap(image);
    pic->setStyleSheet("max-width:100;min-width:100;max-height:100;min-height:100;"
                       "border:4px solid rgb(200,200,200);"
                       "border-radius:52px;background-color:white;");
}

//已登录时再次弹出表单
void UserLogin::passUserInfo(QString userId)
{
    m_id->setText(userId);
    QString headImagePath = QDir::currentPath()+"/userHeads/"+userId;
    QSize size(100, 100);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = QIcon(headImagePath).pixmap(100,100);
    image.setMask(mask);
    pic->setGeometry(200,248,100,100);
    pic->setAlignment(Qt::AlignCenter);
    pic->setPixmap(image);
    pic->setStyleSheet("max-width:100;min-width:100;max-height:100;min-height:100;"
                       "border:4px solid rgb(200,200,200);"
                       "border-radius:52px;background-color:white;");
    QSqlQuery query;
    query.exec(QString("select password from userinfo where userId = '%1' ;").arg(userId));
    query.next();
    m_pwd->setText(query.value(0).toString());
}

void UserLogin::displayReturnValues(QString id, QString pwd)
{
    m_id->setText(id);
    m_pwd->setText(pwd);
}

void UserLogin::on_timeout()
{
    QPropertyAnimation *animation = new QPropertyAnimation;
    QPoint start = m_addPic->pos();
    QPoint end = start + QPoint(-100,0);
    animation->setTargetObject(m_addPic);
    animation->setPropertyName("pos");
    animation->setDuration(300);
    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->start();

    //关闭计时器
    m_timer->stop();
}

bool UserLogin::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == pic)
    {
        if(event->type() == QEvent::Enter)
        {
                if(m_addPic->x() == 225)
                {
                    QPropertyAnimation *animation = new QPropertyAnimation;
                    QPoint start = m_addPic->pos();
                    QPoint end = start + QPoint(100,0);
                    animation->setTargetObject(m_addPic);
                    animation->setPropertyName("pos");
                    animation->setDuration(300);
                    animation->setStartValue(start);
                    animation->setEndValue(end);
                    animation->start();
                }
        }
        else if(event->type() == QEvent::Leave)
        {
                m_timer->start(3000);
        }
        else
        {

        }
    }
    else if(obj == m_addPic)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
                QSize size(100, 100);
                QBitmap mask(size);
                QPainter painter(&mask);
                painter.setRenderHint(QPainter::Antialiasing);
                painter.setRenderHint(QPainter::SmoothPixmapTransform);
                painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
                painter.setBrush(QColor(0, 0, 0));
                painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
                QPixmap image = QIcon(":/icon/res/icon (2).png").pixmap(75,75);
                pic->setPixmap(image);
                m_id->clear();
                m_pwd->clear();
                InformationWindow *logOut = new InformationWindow("登出成功!");
                logOut->show();
                logOut->showInstantly();
                emit loginSuccess("");
            }
    }

    return QWidget::eventFilter(obj,event);
}

void UserLogin::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}

void UserLogin::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
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

QLineEdit *UserLogin::id() const
{
    return m_id;
}



