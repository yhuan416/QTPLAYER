#include "videoplayer.h"

#include <sys/types.h>
#include <dirent.h>

VIDEOPLAYER::VIDEOPLAYER(QWidget *parent):PLAYERWidget(parent)
{
    FULL = false;//全屏状态

    //添加视频
    QString path = "/home/yhuan/video";//路径
//    QString path = "VIDEO";//路径

    //循环读取文件夹下的素材
    DIR *dir = opendir(path.toUtf8().data());
    if (dir == nullptr)
    {
        qDebug() << "read video error";
        this->close();
    }

    struct dirent *file;
    QString tname;
    QString ttype;

    QListWidgetItem *itmp;
    QIcon *icon;

    while((file = readdir(dir)) != nullptr)
    {
        tname = file->d_name;
        ttype = file->d_type;

        if (ttype == "DT_DIR" || tname.at(0) == '.')//如果是文件夹 or 是隐藏文件
        {
            continue;//读下一个
        }

        if (tname.indexOf(".mp4") != -1)//是MP4文件
        {
            QString p(path + "/" + tname);

            //获取图标
            icon = new QIcon(p.split(".").at(0) + ".jpg");

            itmp = new QListWidgetItem(*icon , p);
            AddListItem(itmp);
        }
    }

    Path = GetListItem(0);
}


//单击全屏
void VIDEOPLAYER::mousePressEvent(QMouseEvent *event)
{
    //判断点击的区域
    if (FULL == false && (event->x() > 600 || event->y() > 360))
    {
        //不是点击播放区域
        return;
    }

    //开始全屏播放
    if (FULL == false)
    {
        HideThis();

        FULL = true;

        ReadyPlay();

        //调整进度
        player->write("seek ");
        player->write(QString::number(cSec - 3).toUtf8().data());
        player->write(" \n");
    }
    else
    {
        ShowPlayer();

        FULL = false;

        //开始播放
        ReadyPlay();

        //调整进度
        player->write("seek ");
        player->write(QString::number(cSec - 3).toUtf8().data());
        player->write(" \n");
    }

}

void VIDEOPLAYER::ReadyPlay()
{
    if (run != 0)
    {
        player->kill();
        player->waitForFinished();
    }

    if (FULL == false)
    {
        player->start("mplayer -slave -quiet -zoom -x 600 -y 360 " + Path);
    }
    else
    {
        player->start("mplayer -slave -quiet -zoom -x 800 -y 480 " + Path);
    }

    ctrl->SetPlayStatus(1);

    //调整音量
    player->write("volume ");
    player->write(QString::number(sound).toUtf8().data());
    player->write(" 1\n");
    ctrl->SetSOUND(sound);

    //获取播放文件的长度
    player->write("get_time_length \n");

    t->start(1000);//定时器

    run = 1;//播放状态

    //如果当前是静音状态
    if (mute == true)
    {
        player->write("mute 1\n");//开启静音
    }
    else
    {
        player->write("mute 0\n");//
    }
}

