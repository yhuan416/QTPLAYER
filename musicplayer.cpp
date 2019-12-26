#include "musicplayer.h"

#include <sys/types.h>
#include <dirent.h>

#include <QFile>

MUSICPLAYER::MUSICPLAYER(QWidget *parent):PLAYERWidget(parent)
{
    //添加视频
    QString path = "/home/yhuan/music";//路径
//    QString path = "MUSIC";//路径

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

    while((file = readdir(dir)) != nullptr)
    {
        tname = file->d_name;
        ttype = file->d_type;

        if (ttype == "DT_DIR" || tname.at(0) == '.')//如果是文件夹 or 是隐藏文件
        {
            continue;//读下一个
        }

        if (tname.indexOf(".mp3") != -1)//是MP4文件
        {
            AddListItem(path + "/" + tname);
        }
    }

    //设置窗体
    label = new QLabel(this);
    label->setGeometry(0, 0, 600, 120);
    label->setStyleSheet("color: rgb(52, 101, 164);");

    //设置窗体的字体
    QFont ft;
    ft.setPointSize(32);
    label->setFont(ft);


    list = new QListWidget(this);
    list->setGeometry(0, 120, 600, 240);

    ft.setPointSize(12);
    list->setFont(ft);
    list->setStyleSheet("color: rgb(52, 101, 164);");

    list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Path = GetListItem(0);
}

void MUSICPLAYER::ReadyPlay()
{
    if (run != 0)
    {
        player->kill();
        player->waitForFinished();
    }

    label->setText(Path);

    lrc.clear();
    list->clear();

    cline = 0;

    HasLrc = false;

    //读取歌词
    QFile file(Path.split(".").at(0)+".lrc");
    if (file.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        HasLrc = true;

        while(!file.atEnd())
        {
            QString line = file.readLine();

            QString TIME = line.split("]").at(0).split("[").at(1);

            QString MIN = TIME.split(":").at(0);
            QString SEC = TIME.split(":").at(1);

            lrc.append(MIN.toInt() * 60 + SEC.toDouble());
            list->addItem(line.split("]").at(1));
        }
    }
    else
    {
        list->addItem("当前音乐暂无歌词");
    }

    //开始播放
    player->start("mplayer -slave -quiet " + Path);

    ctrl->SetPlayStatus(1);

    //调整音量
    player->write("volume ");
    player->write(QString::number(sound).toUtf8().data());
    player->write(" 1\n");
    ctrl->SetSOUND(sound);

    //获取播放文件的长度
    player->write("get_time_length \n");

    t->start(500);//定时器

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

void MUSICPLAYER::loopDo()
{
    //进度条
    ctrl->SetJINDU((int)(100 * cSec/fSec));//移动进度条

    if (HasLrc == false)
    {
        return;
    }

    //匹配歌词
    //判断当前歌词的进度
    if (cSec > lrc.at(cline))
    {
        //判断下一句歌词是否已经到达
        for (int i = cline; i < lrc.count(); i++) {
            if (cSec < lrc.at(i))
            {
                cline = i -1;

                if (cline < 0)
                {
                    cline = 0;
                }

                break;
            }
        }
    }
    else if (cSec < lrc.at(cline))
    {
        //往前找
        for (int i = cline; i >= 0; i--) {
            if (cSec > lrc.at(i))
            {
                cline = i + 1;

                if (cline >= lrc.count())
                {
                    cline = lrc.count() - 1;
                }

                break;
            }
        }
    }

    //歌词
    list->setCurrentRow(cline);
    list->scrollToItem(list->currentItem(), QAbstractItemView::PositionAtCenter);
}

void MUSICPLAYER::ShowPlayer()
{
    list->clear();
    label->setText(GetListItem(0));

    MyShow();
}

