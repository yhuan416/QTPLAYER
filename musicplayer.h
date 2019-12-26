#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "playerwidget.h"

#include <QLabel>
#include <QListWidget>

class MUSICPLAYER : public PLAYERWidget
{
public:
    MUSICPLAYER(QWidget *parent = nullptr);

    void ReadyPlay() override;
    void loopDo() override;
    void ShowPlayer() override;

private:
    bool HasLrc;
    QList<double> lrc;//歌词

    QLabel *label;
    QListWidget *list;

    int cline;
};

#endif // MUSICPLAYER_H
