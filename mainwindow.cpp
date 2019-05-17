#include <QFileDialog>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardPaths>

#include <math.h>
#include <iostream>

#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playlistitemmodel.h"
#include "utils/signalbus.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint);

  this->model = new PlaylistItemModel(this);
  this->ui->trackListView->setModel(this->model);

  this->settingsFileName =
      QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)
          .first() +
      "/picaster.ini";
  this->loadSettings();

  this->connect(SignalBus::getInstance(), &SignalBus::jackStarted, this,
                &MainWindow::jackStarted);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addStopPushButton_clicked() {
  int row = this->model->rowCount();
  this->model->insertRow(row);
  QModelIndex index = this->model->index(row, 0);
  this->model->setData(index, QString(">> Stop player <<"));
  this->model->addItem(new ControlItem());
}

void MainWindow::on_addMusicPushButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Add music"),
                                                  this->musicDir, tr("*.mp3" /*"*.mp3, *.mp2, *.ogg, *.oga, *.wav, *.aiff, *.au, *.txt, *.cue, *.avi, *.wma, *.ape, *.mpc, *.aac, *.mp4, *.m4a, *.m4b, *.m4p, *.flac, *.spx, *.opus"*/));
  if (fileName != nullptr) {
    QFileInfo fileInfo = QFileInfo(fileName);
    TagLib::FileRef f(fileName.toStdString().c_str());
    QString item;
    if (!f.isNull() && f.tag()) {
      TagLib::Tag* tag = f.tag();
      if (tag->artist().isEmpty() || tag->title().isEmpty()) {
        item = "[Invalid tag] " + fileInfo.fileName();
      } else {
        item = tag->artist().toCString(true) + QString(" - ") +
               tag->title().toCString(true);
      }

      TagLib::AudioProperties* audio_properties = f.audioProperties();

      int seconds = audio_properties->length() % 60;
      int minutes = (audio_properties->length() - seconds) / 60;

      QString time(" [%1:%2]");
      item += time.arg(minutes).arg(seconds, 2, 10, QChar('0'));
    } else {
      item = fileInfo.fileName();
    }
    this->model->addItem(new TrackItem(item));
    this->musicDir = fileInfo.absolutePath();
  }
}

void MainWindow::loadSettings() {
  QSettings settings(this->settingsFileName, QSettings::IniFormat);

  this->musicDir = settings
                       .value("musicDir", QStandardPaths::standardLocations(
                                              QStandardPaths::MusicLocation)
                                              .first())
                       .toString();

  int length = settings.value("playlist/length", 0).toInt();
  for (int i = 0; i < length; i++) {
    PlaylistItemType playlistItemType = static_cast<PlaylistItemType>(
        settings.value(QString("playlist/%1.type").arg(i)).toInt());
    QString value =
        settings.value(QString("playlist/%1.value").arg(i)).toString();
    switch (playlistItemType) {
      case TRACK:
        this->model->addItem(new TrackItem(value));
        break;
      case CONTROL:
        this->model->addItem(new ControlItem());
        break;
    }
  }
}

void MainWindow::saveSettings() {
  QSettings settings(this->settingsFileName, QSettings::IniFormat);

  settings.setValue("musicDir", this->musicDir);

  int count = this->model->rowCount();
  const QList<PlaylistItem*> list = this->model->list();
  settings.setValue("playlist/length", count);
  for (int i = 0; i < count; i++) {
    PlaylistItemType type = list.at(i)->getType();
    settings.setValue(QString("playlist/%1.type").arg(i), type);
    QString key = QString("playlist/%1.value").arg(i);
    switch (type) {
      case TRACK:
        settings.setValue(key, static_cast<TrackItem*>(list.at(i))->getPath());
        break;
      case CONTROL:
        settings.setValue(key, "");
    }
  }
  std::cerr << this->settingsFileName.toStdString() << std::endl;
}

void MainWindow::on_exitButton_clicked() {
  this->saveSettings();
  QApplication::quit();
}

void MainWindow::on_jackButton_toggled(bool checked) {
  emit SignalBus::getInstance()->startJack(checked);
}

void MainWindow::jackStarted(bool started) {
  this->ui->playButton->setEnabled(started);
  this->ui->exitButton->setEnabled(!started);
}

void MainWindow::on_playButton_clicked() {
  this->ui->playButton->setEnabled(false);
  this->ui->jackButton->setEnabled(false);
  this->ui->stopButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked() {
  this->ui->playButton->setEnabled(true);
  this->ui->jackButton->setEnabled(true);
  this->ui->stopButton->setEnabled(false);
}

void MainWindow::on_removeItemButton_clicked() {
  if (this->ui->trackListView->selectionModel()->hasSelection()) {
    int row = this->ui->trackListView->currentIndex().row();
    this->model->removeItem(row);
  }
}

void MainWindow::on_djVolumeDial_valueChanged(int value) {
  long double f_value = value / static_cast<long double>(10.0);
  long double db_value = 65 * log10(f_value);
  emit SignalBus::getInstance()->djVolumeChanged(db_value);
}
