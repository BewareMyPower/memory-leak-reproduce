#ifndef EXERCISEUTIL_H
#define EXERCISEUTIL_H

#include <pulsar/Client.h>
#include <pulsar/Logger.h>

#include <thread>
#include <signal.h>

#include "./LogUtils.h"

DECLARE_LOG_OBJECT()

static std::string pulsar_url = "pulsar://host.docker.internal:6650/";
static pulsar::Client client(pulsar_url);

#endif 
