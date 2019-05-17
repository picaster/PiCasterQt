#include "jackclient.h"
#include "../utils/signalbus.h"

#include <iostream>

JackClient* JackClient::instance = nullptr;

static int
processCallbackStatic(jack_nframes_t nframes, void* arg)
{
  auto jackClient = static_cast<JackClient*>(arg);
  return jackClient->processCallback(nframes);
}

JackClient*
JackClient::getInstance()
{
  if (JackClient::instance == nullptr) {
    JackClient::instance = new JackClient();
  }
  return JackClient::instance;
}

JackClient::JackClient()
{
  connect(SignalBus::getInstance(),
          &SignalBus::startJack,
          this,
          &JackClient::connectToServer);
}

void
JackClient::connectToServer(bool checked)
{
  if (checked) {
    jack_options_t options = JackNoStartServer;
    jack_status_t status;
    this->client = jack_client_open("picaster", options, &status);
    if (client == nullptr) {
      // We're in trouble
      std::cerr << "Error : cannot connect to jack" << std::endl;
      return;
    }
    jack_set_process_callback(client, processCallbackStatic, this);
    jack_activate(client);
    emit SignalBus::getInstance()->jackStarted(true);
    this->registerModules();
  } else {
    jack_client_close(client);
    client = nullptr;
    emit SignalBus::getInstance()->jackStarted(false);
  }
}

int
JackClient::processCallback(jack_nframes_t nframes)
{
  for (int i = 0; i < modules.size(); i++) {
    modules.at(i)->process(nframes);
  }

  return 0;
}

void
JackClient::registerModules()
{
  micFaderModule = new JackFaderModule("mic", this);
}

void
JackClient::registerModule(JackModule* module)
{
  modules.append(module);
}

JackPort*
JackClient::registerPort(QString name, JackPortFlags jackPortFlags)
{
  auto port = jack_port_register(
    client, name.toUtf8().data(), JACK_DEFAULT_AUDIO_TYPE, jackPortFlags, 0);
  return new JackPort(port);
}

JackPort*
JackClient::getSystemPort(QString name)
{
  return new JackPort(
    jack_port_by_name(client, ("system:" + name).toUtf8().data()));
}

void
JackClient::connectPorts(JackPort* source, JackPort* destination)
{
  jack_connect(client, source->getPortName(), destination->getPortName());
}
