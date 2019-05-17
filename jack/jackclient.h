#ifndef JACKCLIENT_H
#define JACKCLIENT_H

#include <QObject>

#include <jack/jack.h>

#include "jackfadermodule.h"
#include "jackport.h"

#include "../utils/signalbus.h"

class JackClient : public QObject
{
private:
  static JackClient* instance;
  jack_client_t* client;
  QList<JackModule*> modules;

private:
  JackClient();

public:
  static JackClient* getInstance();
  int processCallback(jack_nframes_t nframes);
  JackPort* registerPort(QString name, JackPortFlags jackPortFlags);
  JackPort* getSystemPort(QString name);
  void connectPorts(JackPort* source, JackPort* destination);
  void registerModule(JackModule* module);

private:
  void connectToServer(bool checked);
  void registerModules();
  JackFaderModule* micFaderModule;
};

#endif // JACKCLIENT_H
