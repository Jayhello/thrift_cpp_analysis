//
// Created by wenwen on 2023/6/18.
//

#pragma once

#include "XYTransport.h"

namespace xy{

class TSocket : public ITransport{
public:
    TSocket(const std::string& ip, int port):sIp_(ip), iPort_(port){}

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

private:
    std::string  sIp_;
    int          iPort_;

    int          fd_         = -1;
    bool         bConnected_ = false;
};  // TSocket


} // xy
