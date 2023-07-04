//
// Created by wenwen on 2023/7/2.
//

#include "connected_client.h"
#include "logging.h"

namespace xy{

TConnectedClient::~TConnectedClient() {
}

void TConnectedClient::run(){
    for (bool done = false; !done;){
        try {
//            if (!processor_->process(inputProtocol_, outputProtocol_, opaqueContext_)) {
            if (!processor_->process(inputProtocol_, outputProtocol_, nullptr)) {
                break;
            }
        }catch (const std::exception& ttx) {
            error("client run err: %s", ttx.what());
            done = true;
        }
    }

    cleanup();
}

void TConnectedClient::cleanup(){
    inputProtocol_->getTransport()->close();

    outputProtocol_->getTransport()->close();

    client_->close();
}

} // xy
