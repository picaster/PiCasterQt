#ifndef SIGNALBUS_H
#define SIGNALBUS_H

#include <QObject>

class SignalBus : public QObject
{
  Q_OBJECT

private:
  static SignalBus* instance;

public:
  explicit SignalBus(QObject* parent = nullptr);
  static SignalBus* getInstance();

signals:
  void startJack(bool checked);
  void jackStarted(bool started);
  void djVolumeChanged(long double dbValue);

public slots:
};

#endif // SIGNALBUS_H
