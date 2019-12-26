#include "playerwidget.h"
#include "ui_playerwidget.h"

PLAYERWidget::PLAYERWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PLAYERWidget)
{
    ui->setupUi(this);

    //控制台//////////////////////////////////////////////////////////////////////////

    ctrl = new CTRLWidget(this);
    ctrl->move(0, 360);

    //按键信号
    connect(ctrl, SIGNAL(SigPressPlay()), this, SLOT(PressPlay()));
    connect(ctrl, SIGNAL(SigPressNext()), this, SLOT(PressNext()));
    connect(ctrl, SIGNAL(SigPressPrev()), this, SLOT(PressPrev()));
    connect(ctrl, SIGNAL(SigPressFG()), this, SLOT(PressFG()));
    connect(ctrl, SIGNAL(SigPressFB()), this, SLOT(PressFB()));

    //静音
    connect(ctrl, SIGNAL(SigPressMUTE()), this, SLOT(PressMUTE()));

    //退出
    connect(ctrl, SIGNAL(SigPressBACK()), this, SLOT(PressBACK()));

    connect(ctrl, SIGNAL(SigPressedJINDU()), this, SLOT(PressedJINDU()));
    connect(ctrl, SIGNAL(SigReleasedJINDU(int)), this, SLOT(ReleasedJINDU(int)));
    connect(ctrl, SIGNAL(SigMoveSOUND(int)), this, SLOT(MoveSOUND(int)));

    connect(this, SIGNAL(SigSetBACKColor(QColor)), ctrl, SLOT(SetBACKColor(QColor)));

    //进程////////////////////////////////////////////////////////////////////////////

    player = new QProcess(this);
    connect(player, SIGNAL(readyRead()), this, SLOT(ReadProcess()));
    connect(player, SIGNAL(finished(int , QProcess::ExitStatus)), this, SLOT(FinishProcess()));

    //定时器//////////////////////////////////////////////////////////////////////////

    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(TimeOut()));

    run = 0;
    mute = false;
    sound = 50;
}

PLAYERWidget::~PLAYERWidget()
{
    delete ui;
}

//往列表中添加item
void PLAYERWidget::AddListItem(QString item)
{
    ui->listWidget->addItem(item);
}

void PLAYERWidget::AddListItem(QListWidgetItem *item)
{
    ui->listWidget->addItem(item);
}

//获取制定行的元素
QString PLAYERWidget::GetListItem(int row)
{
    ui->listWidget->setCurrentRow(row);
    return ui->listWidget->currentItem()->text();
}

QString PLAYERWidget::GetListNext()
{
    int i = ui->listWidget->count();
    if (ui->listWidget->currentRow() + 1 == i)
    {
        ui->listWidget->setCurrentRow(0);
    }
    else
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow() + 1);
    }

    return ui->listWidget->currentItem()->text();
}

QString PLAYERWidget::GetListPrev()
{
    int i = ui->listWidget->count();
    if (ui->listWidget->currentRow() - 1 < 0)
    {
        ui->listWidget->setCurrentRow(i - 1);
    }
    else
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow() - 1);
    }

    return ui->listWidget->currentItem()->text();
}

void PLAYERWidget::HideThis()
{
    ctrl->hide();
    ui->listWidget->hide();
}

void PLAYERWidget::MyShow()
{
    this->show();
    ctrl->show();
    ui->listWidget->show();
}

void PLAYERWidget::SetBackColor(QColor c)
{
    emit SigSetBACKColor(c);

    QString cmd("background-color: rgb(%1, %2, %3);");
    cmd = cmd.arg(c.red()).arg(c.green()).arg(c.blue());

    this->setStyleSheet(cmd);
}

void PLAYERWidget::ShowPlayer()
{
    MyShow();
}

//按下播放键
void PLAYERWidget::PressPlay()
{
    if (run == 0)//未在播放状态
    {
        ReadyPlay();//开始播放

        ctrl->SetPlayStatus(1);
    }
    else if (run == 1)//正在播放
    {
        run = 2;//停止播放

        t->stop();

        player->write("pause \n");

        ctrl->SetPlayStatus(0);
    }
    else if (run == 2)//暂停
    {
        run = 1;//开始播放

        player->write("pause \n");

        t->start(500);

        ctrl->SetPlayStatus(1);
    }
}

//获取下一个
void PLAYERWidget::PressNext()
{
    Path = GetListNext();

    ReadyPlay();
}

//获取上一个
void PLAYERWidget::PressPrev()
{
    Path = GetListPrev();

    ReadyPlay();
}

//快进
void PLAYERWidget::PressFG()
{
    if (run == 0)
    {
        return ;
    }

    player->write("seek +5 \n");
    if (run == 2)
    {
        player->write("pause \n");
    }
}

//快退
void PLAYERWidget::PressFB()
{
    if (run == 0)
    {
        return ;
    }

    player->write("seek -5 \n");
    if (run == 2)
    {
        player->write("pause \n");
    }
}

//点击了静音按钮
void PLAYERWidget::PressMUTE()
{
    if (run == 0)
    {
        return;
    }

    if (mute == true)
    {
        player->write("mute 0\n");//关闭静音
        ctrl->SetSOUND(sound);
        mute = false;
    }
    else
    {
        player->write("mute 1\n");//开启静音
        ctrl->SetSOUND(0);
        mute = true;
    }

    if (run == 2)
    {
        player->write("pause \n");//暂停
    }
}

void PLAYERWidget::PressBACK()
{
    if (run != 0)
    {
        player->kill();
        player->waitForFinished();
        run = 0;
    }

    this->hide();
    emit SigShowMain();
}

//停止计时
void PLAYERWidget::PressedJINDU()
{
    t->stop();
}

//松开进度条
void PLAYERWidget::ReleasedJINDU(int)
{
    if (run == 0)
    {
        return;
    }

    int jindu = ctrl->GetJINDU();

    player->write("seek ");
    player->write(QString::number(jindu).toUtf8().data());
    player->write(" 1\n");

    if(run == 2)
    {
        player->write("pause \n");
    }
    else
    {
        t->start(500);//重新开始计时
    }
}

//设置音量
void PLAYERWidget::MoveSOUND(int value)
{
    if (run == 0)
    {
        return ;
    }

    sound = value;

    player->write("volume ");
    player->write(QString::number(sound).toUtf8().data());
    player->write(" 1\n");

    if (run == 2)
        player->write("pause \n");
}

//定时发送一次读数据信号
void PLAYERWidget::TimeOut()
{
    if (run != 1)
    {
        return;
    }
    player->write("get_time_pos \n");
}

//读取进程的数据
void PLAYERWidget::ReadProcess()
{
    QString info = player->readAll();

    if (info.indexOf("ANS_TIME_POSITION") != -1)
    {
        cSec = info.split("=").at(1).toDouble();//获取当前秒数
        loopDo();
    }
    else if (info.indexOf("ANS_LENGTH") != -1)
    {
        fSec = info.split("=").at(1).toDouble();//获取文件秒数
    }
}

//播放完毕
void PLAYERWidget::FinishProcess()
{
    player->waitForFinished();//回收
    run = 0;
    t->stop();//停止定时器
}

//播放状态每秒都要做的事,可被重写
void PLAYERWidget::loopDo()
{
    ctrl->SetJINDU((int)(100 * cSec/fSec));//移动进度条
}

//播放前的准备,可被重写
void PLAYERWidget::ReadyPlay()
{
    if (run != 0)
    {
        player->kill();
        player->waitForFinished();
    }

    player->start("mplayer -slave -quiet -zoom -x 600 -y 360 " + Path);

    run = 1;

    ctrl->SetPlayStatus(1);

    //调整音量
    player->write("volume ");
    player->write(QString::number(sound).toUtf8().data());
    player->write(" 1\n");
    ctrl->SetSOUND(sound);

    //获取播放文件的长度
    player->write("get_time_length \n");

    //如果当前是静音状态
    if (mute == true)
    {
        player->write("mute 1\n");//开启静音
    }
    else
    {
        player->write("mute 0\n");//
    }

    t->start(500);//定时器
}

//选中指定的对象
void PLAYERWidget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Path = item->text();
    ReadyPlay();
}
