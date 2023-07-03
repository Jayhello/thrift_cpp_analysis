//
// Created by wenwen on 2023/6/18.
//

#pragma once

#include "XYTransport.h"

namespace xy{

class TSocket : public ITransport{
public:
    TSocket(const std::string& ip, int port):sIp_(ip), iPort_(port){}

    // 被动的创建(例如服务端 accept返回的)
    TSocket(int fd, const std::string& ip, int port):sIp_(ip), iPort_(port), fd_(fd){}

    virtual bool isOpen() { return bConnected_; }

    virtual bool peek() { return isOpen(); }

    virtual void open();

    virtual void close();

    virtual uint32_t read(uint8_t* buf, uint32_t len);

    virtual void write(const uint8_t* buf, uint32_t len);

    virtual uint32_t readEnd();

    virtual uint32_t writeEnd();

    virtual void flush();

    int connect();

    void setConnTimeout(int ms);

    void setRecvTimeout(int ms);

    void setSendTimeout(int ms);

    void setKeepAlive(bool keepAlive);

private:
    std::string  sIp_;
    int          iPort_;
    int          fd_         = -1;
    bool         bConnected_ = false;
};  // TSocket


} // xy
