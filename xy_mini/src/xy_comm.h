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

class TTransportException : public TException {
public:
    enum TTransportExceptionType {
        UNKNOWN = 0,
        NOT_OPEN = 1,
        TIMED_OUT = 2,
        END_OF_FILE = 3,
        INTERRUPTED = 4,
        BAD_ARGS = 5,
        CORRUPTED_DATA = 6,
        INTERNAL_ERROR = 7
    };

    TTransportException() : TException(), type_(UNKNOWN) {}

    TTransportException(TTransportExceptionType type) : TException(), type_(type) {}

    TTransportException(const std::string& message)
    :TException(message), type_(UNKNOWN) {}

    TTransportException(TTransportExceptionType type, const std::string& message)
    :TException(message), type_(type) {}

    TTransportException(TTransportExceptionType type, const std::string& message, int errno_copy)
//    :TException(message + ": " + TOutput::strerror_s(errno_copy)), type_(type) {}
    :TException(message), type_(type) {}

    virtual ~TTransportException() throw() {}

    TTransportExceptionType getType() const throw() { return type_; }

    virtual const char* what() const throw();

protected:
    /** Just like strerror_r but returns a C++ string object. */
    std::string strerror_s(int errno_copy);
    TTransportExceptionType type_;
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
