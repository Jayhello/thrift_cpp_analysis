//
// Created by wenwen on 2023/7/2.
//
#pragma once

#include "xy_concurrency.h"
#include "xy_processor.h"
#include "XYProtocol.h"
#include "XYTransport.h"

namespace xy{

// This represents a client connected to a TServer.
class TConnectedClient : public Runnable {
public:
    TConnectedClient(const std::shared_ptr<TProcessor>& processor,
                     const std::shared_ptr<IProtocol>& inputProtocol,
                     const std::shared_ptr<IProtocol>& outputProtocol,
//                     const stdcxx::shared_ptr<apache::thrift::server::TServerEventHandler>& eventHandler,
                    const std::shared_ptr<ITransport>& client)
                    :processor_(processor),
                    inputProtocol_(inputProtocol),
                    outputProtocol_(outputProtocol),
                    client_(client){}

    virtual ~TConnectedClient();

    virtual void run();

protected:
    virtual void cleanup();

private:

    std::shared_ptr<TProcessor> processor_;
    std::shared_ptr<IProtocol>  inputProtocol_;
    std::shared_ptr<IProtocol>  outputProtocol_;
//    stdcxx::shared_ptr<apache::thrift::server::TServerEventHandler> eventHandler_;
    std::shared_ptr<ITransport> client_;
};


} // xy
