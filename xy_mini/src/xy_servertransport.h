//
// Created by wenwen on 2023/7/2.
//
#pragma once
#include <memory>
#include <functional>
#include <exception>
#include "XYTransport.h"
#include "xy_comm.h"

namespace xy{

class TServerTransport {
public:
    virtual ~TServerTransport() {}

    virtual void listen() {}

    std::shared_ptr<ITransport> accept() {
        std::shared_ptr<ITransport> result = acceptImpl();
        if (!result) {
            throw TException("accept() may not return NULL");
        }
        return result;
    }

    virtual void interrupt() {}
    virtual void interruptChildren() {}
    virtual int getSocketFD() { return -1; }

    virtual void close() = 0;
protected:
    TServerTransport() {}

    virtual std::shared_ptr<ITransport> acceptImpl() = 0;
};

class TServerSocket : public TServerTransport {
public:
    typedef std::function<void(int fd)> socket_func_t;

    const static int DEFAULT_BACKLOG = 1024;

    TServerSocket(int port);

    virtual ~TServerSocket();

    void setSendTimeout(int sendTimeout);
    void setRecvTimeout(int recvTimeout);

    int getSocketFD() { return serverSocket_; }

    int getPort(){return port_;}

    virtual void listen();
//    void interrupt();
//    void interruptChildren();
    void close();

protected:
    std::shared_ptr<ITransport> acceptImpl();
//    virtual std::shared_ptr<TSocket> createSocket(int client);

private:
//    void notify(int notifySock);

    int port_;
    std::string address_;
    std::string path_;
    int serverSocket_;
    int sendTimeout_;
    int recvTimeout_;
    int accTimeout_;

    socket_func_t listenCallback_;
    socket_func_t acceptCallback_;
};


} // xy
