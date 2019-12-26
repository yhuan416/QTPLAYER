#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QDebug>

#include <QWidget>
#include <QListWidgetItem>

#include <QProcess>
#include <QTimer>

#include "ctrlwidget.h"

namespace Ui {
class PLAYERWidget;
}

class PLAYERWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PLAYERWidget(QWidget *parent = nullptr);
    virtual ~PLAYERWidget();

    void AddListItem(QString);//往list中添加对象
    void AddListItem(QListWidgetItem *);

    QString GetListItem(int);//获取对象

    QString GetListNext();//上一个/下一个
    QString GetListPrev();

    void HideThis();//隐藏自身

    void MyShow();//显示自身

signals:
    void SigShowMain();//跳转到主界面信号
    void SigSetBACKColor(QColor);//改变背景颜色信号

public slots:
    virtual void ShowPlayer();//显示自身

    void PressPlay();//按下按键
    void PressNext();
    void PressPrev();
    void PressFG();
    void PressFB();

    void PressMUTE();//静音按钮
    void PressBACK();//退出按钮

    void PressedJINDU();//开始拖动进度条
    void ReleasedJINDU(int);//结束拖动进度条

    void MoveSOUND(int);//拖动音量条

    void TimeOut();//定时器到时

    void ReadProcess();//进程信号槽函数
    void FinishProcess();

    void SetBackColor(QColor);//背景颜色

protected:
    virtual void loopDo();//播放中要做的事
    virtual void ReadyPlay();//播放前的准备

    QColor backColor;

    Ui::PLAYERWidget *ui;

    //控制台
    CTRLWidget *ctrl;

    //播放线程
    QProcess *player;

    //定时器
    QTimer *t;

    QString Path;

    //音量
    int sound;
    bool mute;

    //播放状态
    // 0 未开始, 1 播放中, 2 暂停中
    int run;

    double cSec;//当前播放的秒数
    double fSec;//播放文件的秒数
private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // PLAYERWIDGET_H
