#ifndef CTRLWIDGET_H
#define CTRLWIDGET_H

#include <QWidget>

namespace Ui {
class CTRLWidget;
}

class CTRLWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CTRLWidget(QWidget *parent = nullptr);
    ~CTRLWidget();

    void SetJINDU(int);
    int GetJINDU();

    void SetSOUND(int);
    int GetSOUND();

    void SetPlayStatus(int);

signals:
    void SigPressPlay();//播放按钮
    void SigPressNext();//下一首按钮
    void SigPressPrev();//上一首按钮
    void SigPressFG();//快进按钮
    void SigPressFB();//快退按钮

    void SigPressMUTE();//静音按钮
    void SigPressBACK();//退出按钮

    void SigPressedJINDU();//开始拖动进度条
    void SigReleasedJINDU(int);//结束拖动进度条

    void SigMoveSOUND(int);//拖动音量条

private slots:
    void SetBACKColor(QColor);

    void on_PLAY_clicked();

    void on_NEXT_clicked();

    void on_PREV_clicked();

    void on_FASTGO_clicked();

    void on_FASTBACK_clicked();

    void on_RETURN_clicked();

    void on_MUTE_clicked();

    void on_JINDU_sliderPressed();

    void on_JINDU_sliderReleased();

    void on_SOUND_sliderMoved(int position);

    void on_SOUND_sliderReleased();

    void on_SOUND_valueChanged(int value);

private:
    Ui::CTRLWidget *ui;
};

#endif // CTRLWIDGET_H
