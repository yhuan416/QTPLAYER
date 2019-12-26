#include "mainwindow.h"
#include "playerwidget.h"
#include "videoplayer.h"
#include "musicplayer.h"

#include <QDebug>

#include <QApplication>

#include <QColor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    VIDEOPLAYER v;

    QObject::connect(&w, SIGNAL(SigShowVideo()), &v, SLOT(ShowPlayer()));
    QObject::connect(&v, SIGNAL(SigShowMain()), &w, SLOT(Show_Main()));
    QObject::connect(&w, SIGNAL(SigSetBack(QColor)), &v, SLOT(SetBackColor(QColor)));

    MUSICPLAYER m;

    QObject::connect(&w, SIGNAL(SigShowMusic()), &m, SLOT(ShowPlayer()));
    QObject::connect(&m, SIGNAL(SigShowMain()), &w, SLOT(Show_Main()));
    QObject::connect(&w, SIGNAL(SigSetBack(QColor)), &m, SLOT(SetBackColor(QColor)));

    return a.exec();
}
