#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "playerwidget.h"
#include <QMouseEvent>

class VIDEOPLAYER: public PLAYERWidget
{
public:
    VIDEOPLAYER(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;

    void ReadyPlay() override;

private:
    bool FULL;
};

#endif // VIDEOPLAYER_H
