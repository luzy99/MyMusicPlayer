#include "createsonglistdialog.h"
#include<QDebug>

CreateSongListDialog::CreateSongListDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMaximumSize(QSize(400,200));
    this->setMinimumSize(QSize(400,200));
//    QPalette pal_windows(palette());
//    pal_windows.setColor(QPalette::Background, QColor(0, 255, 255));
//    this->setPalette(pal_windows);
    this->setStyleSheet("background-color:rgb(255,255,255)");

    //label设置
    label = new QLabel(this);
    label->setText("新建歌单");
    label->setGeometry(0, 0, 100, 40 );
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont("微软雅黑",11,QFont::Thin));

    //line设置  qt中line的本质是qframe衍生出来的
    line = new QFrame(this);
    line->setStyleSheet("QFrame{"
                        "border:none;"
                        "background-color:#DBD0D0;"
                        "max-height : 1px;}");
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setGeometry(0,40,400,1);

    //lineEdit设置
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(60,70,280,40);
    lineEdit->setPlaceholderText("歌单名称");
    lineEdit->setFont(QFont("微软雅黑",11,QFont::Thin));

    //两个按钮的设置
    createButton = new QPushButton(this);
    cancelButton = new QPushButton(this);

    createButton->setGeometry(60,140,80,35);
    cancelButton->setGeometry(260,140,80,35);

    createButton->setText("创建");
    cancelButton->setText("取消");

    createButton->setFont(QFont("微软雅黑",11,QFont::Thin));
    cancelButton->setFont(QFont("微软雅黑",11,QFont::Thin));

    createButton->setStyleSheet("QPushButton{"
                                "background-color:#BF1202;"
                                "border-style:outset;"
                                "border-color:#A50E01;"
                                "border-width: 2px;"
                                "color:#FFFFFF;"
                                "}");//边框颜色貌似会自动被调浅，选择更深一点的颜色

    cancelButton->setStyleSheet("QPushButton{"
                                "background-color:#FFFFFF;"
                                "border-style:outset;"
                                "border-color:#A50E01;"
                                "border-width: 1px;"
                                "color:#BF1202;"
                                "}");

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

//关闭对话框且返回其结果
void CreateSongListDialog::on_createButtonClicked()
{
    this->done(0);
}
void CreateSongListDialog::on_cancelButtonClicked()
{
    this->done(1);
}
