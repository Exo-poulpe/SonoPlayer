#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Dialog *dlg = new Dialog();
    if (dlg->exec() == QDialog::Accepted)
    {
        ui->setupUi(this);
        QPalette pal = this->ui->btnClose->palette();
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        this->ui->btnClose->setPalette(pal);
        this->ui->btnClose->update();
        #ifdef Q_OS_WIN32
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint );
        #endif
        if(dlg->GetPath() != "")
        {
            QRect rec = QApplication::desktop()->screenGeometry();
            this->move(rec.width()-240,rec.height()-135);
            //tmr->callOnTimeout(SLOT(UpdateProgressBar()),Qt::AutoConnection);
            QObject::connect(tmr, SIGNAL(timeout()), this, SLOT(UpdateProgressBar()), Qt::AutoConnection);
            connect(player, SIGNAL(player.positionChanged()),this,SLOT(EndOfSong()));
            player->seekableChanged(true);
            this->path = dlg->GetPath();
            this->ui->lblName->setText(CleanName(this->path));
            SetMusicList();
            UpdateLblName(music[pistNumber]);
        }
        else
        {
            QMessageBox *msg = new QMessageBox();
            msg->setText("Nothing selected");
            msg->setWindowTitle("Error");
            msg->exec();
        }
    }
}


QString MainWindow::CleanName(QString path)
{
    QString tmp = path.mid(path.lastIndexOf('/') + 1);
    return tmp;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::EndOfSong()
{
    if(this->player->position() == this->player->duration())
    {
        this->on_btnNext_clicked();
    }
}

void MainWindow::UpdateLblName(QString songName)
{
    this->ui->lblName->setText(songName);
    this->ui->lblName->setToolTip(songName);
}

void MainWindow::on_btnClose_clicked()
{
    this->close();
}

void MainWindow::SetMusicList()
{
    QDir dir(this->path);
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    for (int i=0;i<files.count();i+=1)
    {
        this->music.append(files[i].fileName());
    }
}

void MainWindow::UpdateProgressBar()
{
    this->musicPosition = player->position();
    this->musicDuration = player->duration();
    this->ui->lblDuration->setText(QString::number((double)musicDuration / (60 * 1000),'f',2));
    this->ui->lblPosition->setText(QString::number((double)musicPosition / 100000,'f',2));
    this->ui->prgTime->setMinimum(0);
    this->ui->prgTime->setMaximum(musicDuration);
    this->ui->prgTime->setValue(musicPosition);
    this->ui->prgTime->update();
}

void MainWindow::on_btnPlayPause_clicked()
{
    if(music.count() == 0)
    {
        SetMusicList();
    }
    UpdateLblName(music[pistNumber]);
    if(playPause == false)
    {
        if(!tmr->isActive())
        {
            tmr->start(1000);
        }
        this->ui->btnPlayPause->setIcon(QIcon(":/Icon/DL/pause.png"));
        playPause = true;
        player->setMedia(QUrl::fromLocalFile(this->path + "/" +music[this->pistNumber]));
        player->setVolume(100);
        if(musicPosition != 0)
        {player->setPosition(musicPosition);}
        player->play();
        this->musicDuration = player->duration();

    }else if (playPause == true)
    {
        this->musicPosition = player->position();
        this->ui->btnPlayPause->setIcon(QIcon(":/Icon/DL/play.png"));
        playPause = false;
        player->pause();
    }


    UpdateProgressBar();
}

void MainWindow::on_btnNext_clicked()
{
    if(this->pistNumber + 1 >= music.count())
    {
        this->pistNumber = 0;
    }
    else
    {
        this->pistNumber += 1;
    }
    UpdateLblName(music[pistNumber]);
    this->musicDuration = 0;
    this->musicPosition = 0;
    if(playPause == true)
    {
        player->stop();
        this->playPause = false;
        this->on_btnPlayPause_clicked();
    }else{
        player->stop();
        this->musicDuration = 0;
        this->musicPosition = 0;
    }
}

void MainWindow::on_btnBack_clicked()
{
    if(this->pistNumber - 1 < 0)
    {
        this->pistNumber = music.count() - 1;
    }
    else
    {
        this->pistNumber -= 1;
    }
    UpdateLblName(music[pistNumber]);
    this->musicDuration = 0;
    this->musicPosition = 0;
    if(playPause == true)
    {
        player->stop();
        this->playPause = false;
        this->on_btnPlayPause_clicked();
    }else{
        player->stop();
        this->musicDuration = 0;
        this->musicPosition = 0;
    }

}
