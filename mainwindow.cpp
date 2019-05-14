#include <QFileDialog>
#include <QStandardItemModel>
#include <QSettings>
#include <QStandardPaths>

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->settingsFileName = QApplication::applicationDirPath().left(1) + ":/picaster.ini";
    this->loadSettings();

    ui->trackListWidget->addItem(QString("All about that base"));
    ui->trackListWidget->addItem(QString("Wreking ball"));
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
    QFileDialog::getOpenFileName(this, tr("Add music"), this->musicDir, tr("*.mp3"/*"*.mp3, *.mp2, *.ogg, *.oga, *.wav, *.aiff, *.au, *.txt, *.cue, *.avi, *.wma, *.ape, *.mpc, *.aac, *.mp4, *.m4a, *.m4b, *.m4p, *.flac, *.spx, *.opus"*/));
}

void MainWindow::loadSettings()
{
    QSettings settings(this->settingsFileName, QSettings::NativeFormat);

    this->musicDir = settings.value("musicDir", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()).toString();
}
