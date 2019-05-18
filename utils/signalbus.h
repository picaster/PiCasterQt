#ifndef SIGNALBUS_H
#define SIGNALBUS_H

#include <QObject>

class SignalBus : public QObject {
  Q_OBJECT

 private:
  static SignalBus* instance;

 public:
  explicit SignalBus(QObject* parent = nullptr);
  static SignalBus* getInstance();

 signals:
  void startJack();
  void stopJack();

  void jackStarted();
  void jackStopped();

  void micDialChanged(long double dbValue);

  void muted();
  void unMuted();

  void startPlaylist();
  void stopPlaylist();
  void playlistStarted();
  void playlistStopped();

  void exit();

 public slots:
};

#endif  // SIGNALBUS_H
