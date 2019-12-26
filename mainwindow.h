#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void SigShowVideo();
    void SigShowMusic();
    void SigSetBack(QColor);

private slots:
    void Show_Main();

    void on_VIDEO_clicked();
    void on_MUSIC_clicked();

    void on_BACKColor_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
