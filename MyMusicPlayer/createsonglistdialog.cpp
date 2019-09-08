#include "createsonglistdialog.h"
#include <QBitmap>
#include <QPainter>
#include <QDebug>

CreateSongListDialog::CreateSongListDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    this->setMaximumSize(QSize(400,200));
    this->setMinimumSize(QSize(400,200));
//    QPalette pal_windows(palette());
//    pal_windows.setColor(QPalette::Background, QColor(0, 255, 255));
//    this->setPalette(pal_windows);
    this->setStyleSheet("background-color:rgb(250,250,250);"
                        "border:1px solid rgb(25,25,25);");

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),2,2);
    setMask(bmp);

    //label设置
    label = new QLabel(this);
    label->setText("  新建歌单");
    label->setStyleSheet("color: rgb(250,250,250);"
                         "background:rgb(25,25,25);");
    label->setGeometry(0, 0, 400, 40 );
    label->setAlignment(Qt::AlignVCenter);
    label->setFont(QFont("微软雅黑",11,QFont::Thin));

    //line设置  qt中line的本质是qframe衍生出来的
    line = new QFrame(this);
    line->setStyleSheet("QFrame{"
                        "border:none;"
                        "background-color: rgb(255,192,203);"
                        "max-height : 1px;}");
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setGeometry(0,40,400,1);

    //lineEdit设置
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(60,80,280,40);
    lineEdit->setStyleSheet("QLineEdit {border:1px solid rgb(95,131,210);"
                            "color: rgb(25,25,25);border-radius: 2px;}"
                            "QLineEdit::hover {border:1px solid rgb(95,131,210);background: rgb(255,225,225); "
                            "color: rgb(25,25,25);border-radius: 2px;}");
    lineEdit->setPlaceholderText("歌单名称");
    lineEdit->setFont(QFont("微软雅黑",11,QFont::Thin));

    //两个按钮的设置
    createButton = new QPushButton(this);
    cancelButton = new QPushButton(this);

    createButton->setGeometry(60,140,130,35);
    cancelButton->setGeometry(210,140,130,35);

    createButton->setText("创建");
    cancelButton->setText("取消");

    createButton->setFont(QFont("微软雅黑",11,QFont::Thin));
    cancelButton->setFont(QFont("微软雅黑",11,QFont::Thin));

    createButton->setStyleSheet("QPushButton{border:1px solid rgb(95,131,210); color: rgb(25,25,25);"
                                "background: rgb(230,225,255);border-radius: 2px;}"
                                "QPushButton::hover {border:1px solid rgb(95,131,210);"
                                "background: rgb(255,225,225); border-radius: 2px;}");
    //边框颜色貌似会自动被调浅，选择更深一点的颜色

    cancelButton->setStyleSheet("QPushButton{border:1px solid rgb(95,131,210);background: rgb(225,225,255);border-radius: 2px;}"
                                "QPushButton::hover {border:1px solid rgb(95,131,210);background: rgb(255,225,225); border-radius: 2px;}");

    //关联信号与槽
    connect(createButton, SIGNAL(clicked()),
     this, SLOT(on_createButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), 
    this, SLOT(on_cancelButtonClicked()));
}

QLineEdit *CreateSongListDialog::getLineEdit() const
{
    return lineEdit;
}

void CreateSongListDialog::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}
void CreateSongListDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

//关闭对话框且返回其结果
void CreateSongListDialog::on_createButtonClicked()
{
    this->done(0);
}
void CreateSongListDialog::on_cancelButtonClicked()
{
    this->done(1);
}
