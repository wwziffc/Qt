#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioOutput = new QAudioOutput(format, this);
    mediaPlayer = new QMediaPlayer(this);

    //总时长
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this,
        [=](qint64 durtion){
//        qDebug() << durtion;
        qint64 totalSec = durtion / 1000 /2;
        QString text = QString("%1:%2")
                .arg(totalSec / 60, 2, 10, QChar('0'))
                .arg(totalSec % 60, 2, 10, QChar('0'));
        ui->totalLabel->setText(text);
        ui->playCourseSlider->setRange(0,durtion);//进度条
    });
    //当前时长
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this,
        [=](qint64 pos){
        qint64 totalSec = pos / 1000 /2;
        QString text = QString("%1:%2")
                .arg(totalSec / 60, 2, 10, QChar('0'))
                .arg(totalSec % 60, 2, 10, QChar('0'));
        ui->curLabel->setText(text);
        ui->playCourseSlider->setValue(pos);
    });
    //拖动进度条
    connect(ui->playCourseSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_fileBtn_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this,
                                                  "请选择目录",
                                                  "D:\\Desk\\oooooooooooo\\Qt\\MusicPlayer\\music");
    QDir dir(path);
    QStringList musicList = dir.entryList(QStringList() << "*.mp3" << "*.wav" << "*.m4a");
    qDebug() << musicList;
    ui->listWidget->addItems(musicList);
    ui->listWidget->setCurrentRow(0);
    for(auto file : musicList) {
        playList.append(QUrl::fromLocalFile(path + "/" +file));
    }
//    qDebug() << playList;
//    qDebug() << playList.size();
}


void Widget::on_palyBtn_clicked()
{
    if(playList.empty())
        return;
    switch(mediaPlayer->state()) {
    //停止->播放
    case QMediaPlayer::StoppedState: {
        curPlayIndex = ui->listWidget->currentRow();
        mediaPlayer->setMedia(playList[curPlayIndex]);
        mediaPlayer->play();
        break;
    }
    //播放->暂停
    case QMediaPlayer::PlayingState: {
        mediaPlayer->pause();
        break;
    }
    //暂停->继续播放
    case QMediaPlayer::PausedState: {
        mediaPlayer->play();
        break;
    }

    }

}


void Widget::on_lastBtn_clicked()
{
    if(playList.empty())
        return;
    curPlayIndex--;
    if(curPlayIndex < 0) {
        curPlayIndex = playList.size() - 1;
    }
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setMedia(playList[curPlayIndex]);
    mediaPlayer->play();
}

void Widget::on_nextBtn_clicked()
{
    if(playList.empty())
        return;
    curPlayIndex++;
    if(curPlayIndex >= playList.size()) {
        curPlayIndex = 0;
    }
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setMedia(playList[curPlayIndex]);
    mediaPlayer->play();
}


void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPlayer->setMedia(playList[curPlayIndex]);
    mediaPlayer->play();
}

