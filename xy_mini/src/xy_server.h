//
// Created by wenwen on 2023/7/2.
//
#pragma once
#include "xy_concurrency.h"
#include "xy_servertransport.h"
#include "xy_processor.h"
#include "connected_client.h"

namespace xy{

class TServer : public Runnable {
public:
    virtual ~TServer() {}

    virtual void serve() = 0;

    virtual void stop() {}

    // Allows running the server as a Runnable thread
    virtual void run() { serve(); }

    TServer(const std::shared_ptr<TProcessor>& processor,
            const std::shared_ptr<TServerTransport>& serverTransport,
            const std::shared_ptr<TTransportFactory>& transportFactory,
            const std::shared_ptr<TProtocolFactory>& protocolFactory)
            : processorFactory_(new TSingletonProcessorFactory(processor)),
            serverTransport_(serverTransport),
            inputTransportFactory_(transportFactory),
            outputTransportFactory_(transportFactory),
            inputProtocolFactory_(protocolFactory),
            outputProtocolFactory_(protocolFactory) {}

protected:
    std::shared_ptr<TProcessorFactory> processorFactory_;
    std::shared_ptr<TServerTransport> serverTransport_;

    std::shared_ptr<TTransportFactory> inputTransportFactory_;
    std::shared_ptr<TTransportFactory> outputTransportFactory_;

    std::shared_ptr<TProtocolFactory> inputProtocolFactory_;
    std::shared_ptr<TProtocolFactory> outputProtocolFactory_;
};

class TServerFramework : public TServer {
public:
    TServerFramework(const std::shared_ptr<TProcessor>& processor,
            const std::shared_ptr<TServerTransport>& serverTransport,
            const std::shared_ptr<TTransportFactory>& transportFactory,
            const std::shared_ptr<TProtocolFactory>& protocolFactory):TServer(processor, serverTransport, transportFactory, protocolFactory){}

    virtual ~TServerFramework();

    virtual void serve();

    // 所有的全部中断
    virtual void stop();

protected:

    virtual void onClientConnected(const std::shared_ptr<TConnectedClient>& pClient) = 0;

    virtual void onClientDisconnected(TConnectedClient* pClient) = 0;

private:
    void newlyConnectedClient(const std::shared_ptr<TConnectedClient>& pClient);

    void disposeConnectedClient(TConnectedClient* pClient);
};

class TSimpleServer : public TServerFramework {
public:
    TSimpleServer(const std::shared_ptr<TProcessor>& processor,
                     const std::shared_ptr<TServerTransport>& serverTransport,
                     const std::shared_ptr<TTransportFactory>& transportFactory,
                     const std::shared_ptr<TProtocolFactory>& protocolFactory):TServerFramework(processor, serverTransport, transportFactory, protocolFactory){}

    virtual ~TSimpleServer();
protected:
    virtual void onClientConnected(const std::shared_ptr<TConnectedClient>& pClient);
    virtual void onClientDisconnected(TConnectedClient* pClient);
};

} // xy
