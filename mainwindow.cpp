#include <QFileDialog>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardPaths>

#include <math.h>
#include <iostream>
#include <sstream>

#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playlistitemmodel.h"
#include "utils/mediaplayer.h"
#include "utils/signalbus.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  /* Qt initialization */
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint);

  /* Setting trackListView's model */
  this->model = new PlaylistItemModel(this);
  this->ui->trackListView->setModel(this->model);

  /* Init media player */
  new MediaPlayer(this->ui->trackListView);

  /* Init settings */
  auto localDataLocation =
      QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
  this->settingsFileName = localDataLocation.first() + "/picaster.ini";
  this->settings = new QSettings(this->settingsFileName, QSettings::IniFormat);

  this->loadSettings();

  /* Connect to signals */

  this->connect(SignalBus::getInstance(), &SignalBus::jackStarted, [=]() {
    this->statusBar()->showMessage("Connected to jack server");
  });

  this->connect(SignalBus::getInstance(), &SignalBus::jackStopped, [=]() {
    this->statusBar()->showMessage("Disconnected from jack server");
  });

  this->connect(SignalBus::getInstance(), &SignalBus::exit, [=]() {
    this->statusBar()->showMessage("Good bye !");
    this->saveSettings();
    QApplication::exit();
  });

  this->connect(SignalBus::getInstance(), &SignalBus::micDialChanged,
                [=](int value) {
                  this->settings->setValue("djVolume", value);
                  char* message;
                  asprintf(&message, "Mic gain set to %d dB", value);
                  this->statusBar()->showMessage(message);
                  free(message);
                });

  /* */
  this->statusBar()->showMessage("PiCaster v0.1-alpha ready");
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
    QString label;
    if (!f.isNull() && f.tag()) {
      TagLib::Tag* tag = f.tag();
      if (tag->artist().isEmpty() || tag->title().isEmpty()) {
        label = "[Invalid tag] " + fileInfo.fileName();
      } else {
        label = tag->artist().toCString(true) + QString(" - ") +
                tag->title().toCString(true);
      }

      TagLib::AudioProperties* audio_properties = f.audioProperties();

      int seconds = audio_properties->length() % 60;
      int minutes = (audio_properties->length() - seconds) / 60;

      QString time(" [%1:%2]");
      label += time.arg(minutes).arg(seconds, 2, 10, QChar('0'));
    } else {
      label = fileInfo.fileName();
    }
    this->model->addItem(new TrackItem(label, fileInfo.filePath()));
    this->musicDir = fileInfo.absolutePath();
  }
}

void MainWindow::loadSettings() {
  auto standardMusicLocation =
      QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
  this->musicDir =
      settings->value("musicDir", standardMusicLocation).toString();

  int length = settings->value("playlist/length", 0).toInt();
  for (int i = 0; i < length; i++) {
    PlaylistItemType playlistItemType = static_cast<PlaylistItemType>(
        settings->value(QString("playlist/%1.type").arg(i)).toInt());
    QString label =
        settings->value(QString("playlist/%1.value").arg(i)).toString();
    switch (playlistItemType) {
      case TRACK: {
        QString path =
            settings->value(QString("playlist/%1.path").arg(i)).toString();
        this->model->addItem(new TrackItem(label, path));
        break;
      }
      case CONTROL:
        this->model->addItem(new ControlItem());
        break;
    }
  }

  /* MicDial */
  auto dbValue = settings->value("djVolume").toInt();
  auto f_value = pow(10, dbValue / 65.0f);
  auto value = static_cast<int>(10 * f_value);
  this->ui->djVolumeDial->setValue(value);
}

void MainWindow::saveSettings() {
  settings->setValue("musicDir", this->musicDir);

  int count = this->model->rowCount();
  const QList<PlaylistItem*> list = this->model->list();
  settings->setValue("playlist/length", count);
  for (int i = 0; i < count; i++) {
    PlaylistItemType type = list.at(i)->getType();
    settings->setValue(QString("playlist/%1.type").arg(i), type);
    QString key = QString("playlist/%1.value").arg(i);
    switch (type) {
      case TRACK:
        settings->setValue(key,
                           static_cast<TrackItem*>(list.at(i))->getLabel());
        settings->setValue(QString("playlist/%1.path").arg(i),
                           static_cast<TrackItem*>(list.at(i))->getPath());
        break;
      case CONTROL:
        settings->setValue(key, "");
    }
  }
}

/*
void MainWindow::on_exitButton_clicked() {
  this->saveSettings();
  QApplication::quit();
}
*/

/*
void MainWindow::on_jackButton_toggled(bool checked) {
  emit SignalBus::getInstance()->startJack(checked);
}
*/

/*
void MainWindow::jackStarted(bool started) {
  this->ui->playButton->setEnabled(started);
  this->ui->exitButton->setEnabled(!started);
}
*/
/*
void MainWindow::on_playButton_clicked() {
  this->ui->playButton->setEnabled(false);
  this->ui->jackButton->setEnabled(false);
  this->ui->stopButton->setEnabled(true);
}
*/
/*
void MainWindow::on_stopButton_clicked() {
  this->ui->playButton->setEnabled(true);
  this->ui->jackButton->setEnabled(true);
  this->ui->stopButton->setEnabled(false);
}
*/
void MainWindow::on_removeItemButton_clicked() {
  if (this->ui->trackListView->selectionModel()->hasSelection()) {
    int row = this->ui->trackListView->currentIndex().row();
    this->model->removeItem(row);
  }
}

/*
void MainWindow::on_djVolumeDial_valueChanged(int value) {
  long double f_value = value / static_cast<long double>(10.0);
  long double db_value = 65 * log10(f_value);
  emit SignalBus::getInstance()->djVolumeChanged(db_value);
}
*/
