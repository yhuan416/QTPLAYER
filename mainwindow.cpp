#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>

#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Show_Main()
{
    this->show();
}


void MainWindow::on_VIDEO_clicked()
{
    this->hide();
    emit SigShowVideo();
}

void MainWindow::on_MUSIC_clicked()
{
    this->hide();
    emit SigShowMusic();
}

void MainWindow::on_BACKColor_clicked()
{
    //创建一个QColor
    QColor myColor(204,204,204);

    //创建一个颜色对话框，并指定初始颜色和父对象
    QColorDialog myColorDialog(myColor,this);

    //设置option
//    myColorDialog.setOption(QColorDialog::ShowAlphaChannel);
//    myColorDialog.setOption(QColorDialog::DontUseNativeDialog, false);

    //将对话框显示为模态对话框，阻塞直到用户关闭它
    myColorDialog.exec();

    //获取当前选中的颜色
    myColor = myColorDialog.currentColor();

    if (!myColor.isValid())
    {
        myColor.setRgb(204, 204, 204);
    }

    emit SigSetBack(myColor);

    QString cmd("background-color: rgb(%1, %2, %3);");
    cmd = cmd.arg(myColor.red()).arg(myColor.green()).arg(myColor.blue());

    this->setStyleSheet(cmd);
}
