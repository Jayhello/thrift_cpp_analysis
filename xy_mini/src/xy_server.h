//
// Created by wenwen on 2023/7/2.
//
#pragma once
#include "xy_concurrency.h"

namespace xy{

class TServer : public Runnable {
public:
    virtual ~TServer() {}

    virtual void serve() = 0;

    virtual void stop() {}

    // Allows running the server as a Runnable thread
    virtual void run() { serve(); }

protected:
    TServer();



};

} // xy
