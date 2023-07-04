//
// Created by wenwen on 2023/7/2.
//

#include "xy_server.h"

namespace xy{

void TServerFramework::serve(){
    // Start the server listening
    serverTransport_->listen();

    for(;;){
        try{
            std::shared_ptr<ITransport> client = serverTransport_->accept();

//            {
//                Synchronized sync(mon_);
//                while (clients_ >= limit_)mon_.wait();
//            }

            std::shared_ptr<ITransport> inputTransport = inputTransportFactory_->getTransport(client);
            std::shared_ptr<ITransport> outputTransport = outputTransportFactory_->getTransport(client);
            std::shared_ptr<IProtocol> inputProtocol = inputProtocolFactory_->getProtocol(inputTransport);
            std::shared_ptr<IProtocol> outputProtocol = outputProtocolFactory_->getProtocol(outputTransport);

//            std::shared_ptr<TConnectedClient> pClient = std::make_shared<TConnectedClient>();
            auto delFun = bind(&TServerFramework::disposeConnectedClient, this, std::placeholders::_1);
            auto pNewClient = std::shared_ptr<TConnectedClient>(new TConnectedClient(processorFactory_->getProcessor(), inputProtocol, outputProtocol, client), delFun);
            newlyConnectedClient(pNewClient);   // 子类去执行io

        }catch (const TTransportException& ttx){
            if (ttx.getType() == TTransportException::TIMED_OUT){
                continue;
            }else if (ttx.getType() == TTransportException::END_OF_FILE
            || ttx.getType() == TTransportException::INTERRUPTED) {
                // Server was interrupted.  This only happens when stopping.
                break;
            } else {
                // All other transport exceptions are logged.
                // State of connection is unknown.  Done.
                string errStr = string("TServerTransport died: ") + ttx.what();
//                GlobalOutput(errStr.c_str());
                break;
            }
        }
    }
}

void TServerFramework::stop(){
    serverTransport_->interruptChildren();
    serverTransport_->interrupt();
}

void TServerFramework::newlyConnectedClient(const std::shared_ptr<TConnectedClient>& pClient){
//    {
//        Synchronized sync(mon_);
//        ++clients_;
//        hwm_ = (std::max)(hwm_, clients_);
//    }
    onClientConnected(pClient);
}

// shared_ptr 的析构函数
void TServerFramework::disposeConnectedClient(TConnectedClient* pClient){
    onClientDisconnected(pClient);
    delete pClient;
//    Synchronized sync(mon_);
//    if (limit_ - --clients_ > 0) {
//        mon_.notify();
//    }
}

TServerFramework::~TServerFramework(){
}

TSimpleServer::~TSimpleServer(){
}

void TSimpleServer::onClientConnected(const std::shared_ptr<TConnectedClient>& pClient){
    pClient->run();
}

//TSimpleServer does not track clients so there is nothing to do here.
void TSimpleServer::onClientDisconnected(TConnectedClient* pClient){
}

} // xy
