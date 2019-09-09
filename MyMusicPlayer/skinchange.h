#ifndef SKINCHANGE_H
#define SKINCHANGE_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QMap>

class SkinChange : public QWidget
{
    Q_OBJECT
public:
    explicit SkinChange(QWidget *parent = nullptr);  
    void initQMap();
    void addItems();

signals:
    void colorChanged(QColor);
public slots:
    void on_iTemClicked();
private:
    QMap<int,QColor> m_Color;
    QMap<int,QString> m_ColorName;
    QListWidget *m_pSkinList;
    QWidget *m_pSkinWid;
    QLabel *m_pTipLab;
    QLabel *m_pSkinLab;

};

#endif // SKINCHANGE_H
