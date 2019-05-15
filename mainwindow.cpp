#include <QFileDialog>
#include <QStandardItemModel>
#include <QSettings>
#include <QStandardPaths>

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->settingsFileName = QApplication::applicationDirPath().left(1) + ":/picaster.ini";
    this->loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addStopPushButton_clicked()
{
    this->ui->trackListWidget->addItem(QString(">> Stop player <<"));
}

void MainWindow::on_addMusicPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Add music"), this->musicDir, tr("*.mp3"/*"*.mp3, *.mp2, *.ogg, *.oga, *.wav, *.aiff, *.au, *.txt, *.cue, *.avi, *.wma, *.ape, *.mpc, *.aac, *.mp4, *.m4a, *.m4b, *.m4p, *.flac, *.spx, *.opus"*/));
    if (fileName != nullptr)
    {
        QFileInfo fileInfo = QFileInfo(fileName);
        TagLib::FileRef f(fileName.toStdString().c_str());
        QString item;
        if (!f.isNull() && f.tag())
        {
            TagLib::Tag* tag = f.tag();
            if (tag->artist().isEmpty() || tag->title().isEmpty())
            {
                item = "[Invalid tag] " + fileInfo.fileName();
            }
            else
            {
                item = tag->artist().toCString(true) + QString(" - ") + tag->title().toCString(true);
            }

            TagLib::AudioProperties* audio_properties = f.audioProperties();

            int seconds = audio_properties->length() % 60;
            int minutes = (audio_properties->length() - seconds) / 60;

            QString time(" [%1:%2]");
            item += time.arg(minutes).arg(seconds, 2, 10, QChar('0'));
        }
        else
        {
            item = fileInfo.fileName();
        }
        ui->trackListWidget->addItem(item);
        this->musicDir = fileInfo.absolutePath();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings(this->settingsFileName, QSettings::NativeFormat);

    this->musicDir = settings.value("musicDir", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()).toString();
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_jackButton_toggled(bool checked)
{
    this->ui->playButton->setEnabled(checked);
    this->ui->exitButton->setEnabled(!checked);
}

void MainWindow::on_playButton_clicked()
{
    this->ui->playButton->setEnabled(false);
    this->ui->jackButton->setEnabled(false);
    this->ui->stopButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked()
{
    this->ui->playButton->setEnabled(true);
    this->ui->jackButton->setEnabled(true);
    this->ui->stopButton->setEnabled(false);
}
