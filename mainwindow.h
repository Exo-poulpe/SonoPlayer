#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include <string>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QTimer>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnClose_clicked();

    void on_btnPlayPause_clicked();

    void on_btnNext_clicked();

    void on_btnBack_clicked();

    void UpdateProgressBar();

    void EndOfSong();

private:
    QString CleanName(QString path);
    void SetMusicList();
    void UpdateLblName(QString songName);
    QTimer *tmr = new QTimer;
    Ui::MainWindow *ui;
    QString path;
    QStringList music = QStringList();
    double pistNumber = 0;
    bool playPause = false; // False if not play
    QMediaPlayer *player = new QMediaPlayer();
    qint64 musicDuration = 0;
    int musicPosition = 0;
};
#endif // MAINWINDOW_H
