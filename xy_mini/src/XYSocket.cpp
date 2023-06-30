//
// Created by wenwen on 2023/6/18.
//

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


void TSocket::open(){
    if(bConnected_) return;

    ScopeLog Log;
    if(fd_ < 0){
        fd_ = getTcpSocket();
        Log << "fd:" << fd_;
    }

    int ret = doConnect(fd_, sIp_, iPort_);
    Log << "|connect_ret" << ret;
    if(ret >= 0){
        bConnected_ = true;
    }
}

void TSocket::close(){
    if(fd_ > 0){
        doClose(fd_);
        fd_ = -1;
    }
    bConnected_ = false;
}

uint32_t TSocket::read(uint8_t* buf, uint32_t len){
    ssize_t read_bytes = ::read(fd_, buf, len);
    return read_bytes;
}

void TSocket::write(const uint8_t* buf, uint32_t len){
    ::write(fd_, buf, len);
}

uint32_t TSocket::readEnd(){
    return 0;
}

uint32_t TSocket::writeEnd(){
    return 0;
}

void TSocket::flush(){

}

int TSocket::connect(){
    return 0;
}

} // xy
