//
// Created by wenwen on 2023/6/17.
//
#pragma once
#include <exception>
#include <string>
#include <string>
#include <sys/socket.h>

#define LOCAL_IP    "127.0.0.1"
#define PORT        8880

namespace xy{

class TException : public std::exception {
public:
TException() : message_() {}

TException(const std::string& message) : message_(message) {}

virtual ~TException() throw() {}

virtual const char* what() const throw() {
    if (message_.empty()) {
        return "Default TException.";
    } else {
        return message_.c_str();
    }
}

protected:
    std::string message_;
};

class noncopyable {
protected:
    // 子类可以构造和析构
    noncopyable() = default;
    ~noncopyable() = default;

private:
    noncopyable(const noncopyable &) = delete;
    noncopyable& operator=(const noncopyable &) = delete;
};

using std::string;

// < 0 错误
int getTcpSocket();

// < 0 错误
int doConnect(int fd, const string& sIp, int iPort);

int doBind(int fd, const string& sIp, int iPort);

int doListen(int fd, int n = SOMAXCONN);

int doAccept(int fd, string& sIp, int& iPort);

// create, bind, listen, ret > 0 succ
int createTcpServerSocket(const string& sIp, int iPort);

// < 0 错误 , > 0 写的数据size  (这个write没有返回0的)
// todo read 这里的参数 sData 是否可以优化下, const char* str, len
int doWrite(int fd, const string& sData);

// < 0 错误,  = 0 对方关闭了链接, > 0 写的数据size
int doRead(int fd, string& sData, size_t iReadSize);

// < 0 错误 , > 0 写的数据size  (这个write没有返回0的)
int doSend(int fd, const string& sData);

// < 0 错误,  = 0 对方关闭了链接, > 0 写的数据size
int doRecv(int fd, string& sData, size_t iReadSize);

int doClose(int fd);

int setNonBlock(int fd, bool value = true);

int setReuseAddr(int fd);

int setReusePort(int fd);

int setSocketOpt(int fd, int opt, const void* val, socklen_t opt_len, int level);

uint64_t gettid();

} // xy
