#include "ctrlwidget.h"
#include "ui_ctrlwidget.h"

CTRLWidget::CTRLWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTRLWidget)
{
    ui->setupUi(this);

    //设置进度条的属性
    ui->JINDU->setMaximum(100);
    ui->JINDU->setMinimum(0);
    ui->JINDU->setSingleStep(1);
    ui->JINDU->setValue(0);

    //设置音量条的属性
    ui->SOUND->setMaximum(100);
    ui->SOUND->setMinimum(0);
    ui->SOUND->setSingleStep(1);
    ui->SOUND->setValue(0);

    QMatrix matrix;
    matrix.rotate(180);

    //设置图片
    QPixmap icon(":/player/箭头.png");
    ui->FASTGO->setIcon(icon);

    icon = icon.transformed(matrix, Qt::SmoothTransformation);
    ui->FASTBACK->setIcon(icon);

    icon = QPixmap(":/player/顶端.png");

    matrix.rotate(-90);
    icon = icon.transformed(matrix, Qt::SmoothTransformation);
    ui->NEXT->setIcon(icon);

    matrix.rotate(90);
    icon = icon.transformed(matrix, Qt::SmoothTransformation);
    ui->PREV->setIcon(icon);

    icon = QPixmap(":/player/首页.png");
    ui->RETURN->setIcon(icon);

    icon = QPixmap(":/player/播放.png");
    ui->PLAY->setIcon(icon);
}

CTRLWidget::~CTRLWidget()
{
    delete ui;
}

//外部设置进度条
void CTRLWidget::SetJINDU(int value)
{
    ui->JINDU->setValue(value);
}

//外部获取进度条
int CTRLWidget::GetJINDU()
{
    return ui->JINDU->value();
}

//外部设置音量条
void CTRLWidget::SetSOUND(int value)
{
    ui->SOUND->setValue(value);
}

//外部获取音量条
int CTRLWidget::GetSOUND()
{
    return ui->SOUND->value();
}

//设置播放暂停键
void CTRLWidget::SetPlayStatus(int i)
{
    if (i == 0)
    {
        QPixmap icon = QPixmap(":/player/播放.png");
        ui->PLAY->setIcon(icon);
    }
    else if (i == 1)
    {
        QPixmap icon = QPixmap(":/player/暂停.png");
        ui->PLAY->setIcon(icon);
    }
}

void CTRLWidget::SetBACKColor(QColor c)
{
    QString cmd("background-color: rgb(%1, %2, %3);");
    cmd = cmd.arg(c.red()).arg(c.green()).arg(c.blue());

    this->setStyleSheet(cmd);
}

//发送按下播放键
void CTRLWidget::on_PLAY_clicked()
{
    emit SigPressPlay();
}

//发送按下下一首
void CTRLWidget::on_NEXT_clicked()
{
    emit SigPressNext();
}

//发送按下上一首
void CTRLWidget::on_PREV_clicked()
{
    emit SigPressPrev();
}

//发送按下快进
void CTRLWidget::on_FASTGO_clicked()
{
    emit SigPressFG();
}

//发送按下快退
void CTRLWidget::on_FASTBACK_clicked()
{
    emit SigPressFB();
}

//发送按下退出按钮
void CTRLWidget::on_RETURN_clicked()
{
    emit SigPressBACK();
}

//发送按下静音按钮
void CTRLWidget::on_MUTE_clicked()
{
    emit SigPressMUTE();
}

//开始拖动进度条
void CTRLWidget::on_JINDU_sliderPressed()
{
    emit SigPressedJINDU();
}

//结束拖动进度条
void CTRLWidget::on_JINDU_sliderReleased()
{
    emit SigReleasedJINDU(ui->JINDU->value());
}

//拖动音量条,已弃用
void CTRLWidget::on_SOUND_sliderMoved(int position)
{

}

void CTRLWidget::on_SOUND_sliderReleased()
{
    emit SigMoveSOUND(ui->SOUND->value());
}

void CTRLWidget::on_SOUND_valueChanged(int value)
{
//    emit SigMoveSOUND(value);
}
