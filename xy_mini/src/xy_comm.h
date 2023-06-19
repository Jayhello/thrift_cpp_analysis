//
// Created by wenwen on 2023/6/17.
//
#pragma once
#include <exception>
#include <string>

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

} // xy
