#include "signalbus.h"

SignalBus* SignalBus::instance = nullptr;

SignalBus*
SignalBus::getInstance()
{
  if (SignalBus::instance == nullptr) {
    SignalBus::instance = new SignalBus();
  }
  return SignalBus::instance;
}

SignalBus::SignalBus(QObject* parent)
  : QObject(parent)
{}
