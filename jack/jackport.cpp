#include "jackport.h"

JackPort::JackPort(jack_port_t* port)
{
  this->port = port;
}

const char*
JackPort::getPortName() const
{
  return jack_port_name(port);
}

float*
JackPort::getBuffer(jack_nframes_t nframes)
{
  auto buffer = static_cast<float*>(jack_port_get_buffer(port, nframes));
  return buffer;
}
