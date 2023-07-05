//
// Created by Administrator on 2023/7/5.
//

#include "xy_exception.h"

namespace xy{

const char *TTransportException::what() const throw() {
    if (message_.empty()) {
        switch (type_) {
            case UNKNOWN:
                return "TTransportException: Unknown transport exception";
            case NOT_OPEN:
                return "TTransportException: Transport not open";
            case TIMED_OUT:
                return "TTransportException: Timed out";
            case END_OF_FILE:
                return "TTransportException: End of file";
            case INTERRUPTED:
                return "TTransportException: Interrupted";
            case BAD_ARGS:
                return "TTransportException: Invalid arguments";
            case CORRUPTED_DATA:
                return "TTransportException: Corrupted Data";
            case INTERNAL_ERROR:
                return "TTransportException: Internal error";
            default:
                return "TTransportException: (Invalid exception type)";
        }
    } else {
        return message_.c_str();
    }
}

std::string TApplicationException::toStr()const{
//    std::ostringstream out;
//    out << "TApplicationException(";
//    out << "type=" << type_;
//    out << ", message=" << message_;
//    out << ")";
}

void TApplicationException::printTo(std::ostream& out) const{
    // 不知道为什么编译报错
    out << "TApplicationException(";
    out << "type=" << type_;
    out << ", message=" << message_;
    out << ")";
}

std::ostream& operator<<(std::ostream& out, const TApplicationException& obj){
    obj.printTo(out);
    return out;
}

uint32_t TApplicationException::read(IProtocol* iprot){
    std::string name;
    TType fieldType;
    int16_t fieldId;
    uint32_t len = iprot->readFieldBegin(name, fieldType, fieldId);
    len += iprot->readString(message_);
    len += iprot->readFieldEnd();

    len += iprot->readFieldBegin(name, fieldType, fieldId);
    int32_t type = 0;
    len += iprot->readI32(type);
    type_ = (TApplicationExceptionType)type;
    len += iprot->readFieldEnd();

    len += iprot->readFieldBegin(name, fieldType, fieldId);   // 这个是stop
    len += iprot->readStructEnd();

    return len;
}

uint32_t TApplicationException::write(IProtocol* oprot) const{
    uint32_t len = 0;
    len += oprot->writeStructBegin("TApplicationException");

    len += oprot->writeFieldBegin("message", T_STRING, 1);
    len += oprot->writeString(message_);
    len += oprot->writeFieldEnd();

    len += oprot->writeFieldBegin("type", T_I32, 2);
    len += oprot->writeI32(type_);
    len += oprot->writeFieldEnd();

    len += oprot->writeFieldStop();

    len += oprot->writeStructEnd();

    return len;
}

} // xy
