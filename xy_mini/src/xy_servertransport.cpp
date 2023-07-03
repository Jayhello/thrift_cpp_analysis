//
// Created by wenwen on 2023/7/2.
//

#include "xy_servertransport.h"
#include "XYSocket.h"
#include "xy_comm.h"
#include "logging.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace xy{

TServerSocket::TServerSocket(int port):port_(port){
}

void TServerSocket::listen(){
    ScopeLog Log;

    serverSocket_ = createTcpServerSocket(LOCAL_IP, port_);
    Log << "fd:" << serverSocket_;
    fatalif(serverSocket_ < 0, "create_sock_bind_fail");
}

void TServerSocket::close(){
    if(serverSocket_ > 0){
        doClose(serverSocket_);
        serverSocket_ = -1;
    }
}

std::shared_ptr<ITransport> TServerSocket::acceptImpl(){
    string sIp;
    int iPort = -1;
    int cfd = doAccept(serverSocket_, sIp, iPort);
    info("accept new client fd: %d, ip: %s, port: %d", cfd, sIp.c_str(), iPort);
    if(cfd < 0){
        // throw exception
        return nullptr;
    }

    return std::shared_ptr<TSocket>(new TSocket(cfd, sIp, iPort));
}

//void TServerSocket::notify(int notifySock);

void TServerSocket::setSendTimeout(int sendTimeout){}
void TServerSocket::setRecvTimeout(int recvTimeout){}

TServerSocket::~TServerSocket(){
    close();
}

} // xy
