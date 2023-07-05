//
// Created by Administrator on 2023/7/5.
//
#pragma once

#include <exception>
#include <iostream>
#include "XYProtocol.h"

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


class TApplicationException : public TException {
public:
    enum TApplicationExceptionType {
        UNKNOWN = 0,
        UNKNOWN_METHOD = 1,
        INVALID_MESSAGE_TYPE = 2,
        WRONG_METHOD_NAME = 3,
        BAD_SEQUENCE_ID = 4,
        MISSING_RESULT = 5,
        INTERNAL_ERROR = 6,
        PROTOCOL_ERROR = 7,
        INVALID_TRANSFORM = 8,
        INVALID_PROTOCOL = 9,
        UNSUPPORTED_CLIENT_TYPE = 10
    };

    TApplicationException() : TException(), type_(UNKNOWN) {}

    TApplicationException(TApplicationExceptionType type) : TException(), type_(type) {}

    TApplicationException(const std::string& message) : TException(message), type_(UNKNOWN) {}

    TApplicationException(TApplicationExceptionType type, const std::string& message)
    : TException(message), type_(type) {}

    virtual ~TApplicationException() throw() {}

    TApplicationExceptionType getType() const { return type_; }

    virtual const char* what() const throw() {
        if (message_.empty()) {
            switch (type_) {
                case UNKNOWN:
                    return "TApplicationException: Unknown application exception";
                case UNKNOWN_METHOD:
                    return "TApplicationException: Unknown method";
                case INVALID_MESSAGE_TYPE:
                    return "TApplicationException: Invalid message type";
                case WRONG_METHOD_NAME:
                    return "TApplicationException: Wrong method name";
                case BAD_SEQUENCE_ID:
                    return "TApplicationException: Bad sequence identifier";
                case MISSING_RESULT:
                    return "TApplicationException: Missing result";
                case INTERNAL_ERROR:
                    return "TApplicationException: Internal error";
                case PROTOCOL_ERROR:
                    return "TApplicationException: Protocol error";
                case INVALID_TRANSFORM:
                    return "TApplicationException: Invalid transform";
                case INVALID_PROTOCOL:
                    return "TApplicationException: Invalid protocol";
                case UNSUPPORTED_CLIENT_TYPE:
                    return "TApplicationException: Unsupported client type";
                default:
                    return "TApplicationException: (Invalid exception type)";
            };
        } else {
            return message_.c_str();
        }
    }

    uint32_t read(IProtocol* iprot);
    uint32_t write(IProtocol* oprot) const;

    virtual void printTo(std::ostream& out) const;

    std::string toStr()const;

protected:
    TApplicationExceptionType type_;
};

std::ostream& operator<<(std::ostream& out, const TApplicationException& obj);

} // xy
