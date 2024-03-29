//
// Created by wenwen on 2023/6/17.
//

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "XYTransport.h"

namespace xy{

enum TType {
    T_STOP       = 0,
    T_VOID       = 1,
    T_BOOL       = 2,
    T_BYTE       = 3,
    T_I08        = 3,
    T_I16        = 6,
    T_I32        = 8,
    T_U64        = 9,
    T_I64        = 10,
    T_DOUBLE     = 4,
    T_STRING     = 11,
    T_UTF7       = 11,
    T_STRUCT     = 12,
    T_MAP        = 13,
    T_SET        = 14,
    T_LIST       = 15,
    T_UTF8       = 16,
    T_UTF16      = 17
};

enum TMessageType {
    T_CALL       = 1,
    T_REPLY      = 2,
    T_EXCEPTION  = 3,
    T_ONEWAY     = 4
};

class IProtocol{
public:
    // 例如一个rpc的消息
    virtual uint32_t readMessageBegin(std::string& name, TMessageType& messageType, int32_t& seqid) = 0;
    virtual uint32_t readMessageEnd() = 0;

    // 例如一个thrift struct
    virtual uint32_t readStructBegin(std::string& name) = 0;
    virtual uint32_t readStructEnd() = 0;

    virtual uint32_t readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId) = 0;

    virtual uint32_t readFieldEnd() = 0;

    uint32_t readBool(bool& value){
        return readBool_virt(value);
    }
    uint32_t readByte(int8_t& byte) {
        return readByte_virt(byte);
    }

    uint32_t readI16(int16_t& i16) {
        return readI16_virt(i16);
    }

    uint32_t readI32(int32_t& i32) {
        return readI32_virt(i32);
    }

    uint32_t readI64(int64_t& i64) {
        return readI64_virt(i64);
    }

    uint32_t readString(std::string& str) {
        return readString_virt(str);
    }

    virtual uint32_t writeMessageBegin(const std::string& name,
                               const TMessageType messageType,
                               const int32_t seqid) = 0;

    virtual uint32_t writeMessageEnd() = 0;

    virtual uint32_t writeStructBegin(const char* name) = 0;

    virtual uint32_t writeStructEnd() = 0;

    virtual uint32_t writeFieldBegin(const std::string& name, const TType fieldType, const int16_t fieldId) = 0;

    virtual uint32_t writeFieldEnd() = 0;

    virtual uint32_t writeFieldStop() = 0;

    uint32_t writeBool(const bool value) {
        return writeBool_virt(value);
    }

    uint32_t writeByte(const int8_t byte) {
        return writeByte_virt(byte);
    }

    uint32_t writeI16(const int16_t i16) {
        return writeI16_virt(i16);
    }

    uint32_t writeI32(const int32_t i32) {
        return writeI32_virt(i32);
    }

    uint32_t writeI64(const int64_t i64) {
        return writeI64_virt(i64);
    }

    uint32_t writeString(const std::string& str) {
        return writeString_virt(str);
    }

    inline std::shared_ptr<ITransport> getTransport() { return pTransport_; }

protected:
    virtual uint32_t readBool_virt(bool& value) = 0;
    virtual uint32_t readByte_virt(int8_t& byte) = 0;
    virtual uint32_t readI16_virt(int16_t& i16) = 0;
    virtual uint32_t readI32_virt(int32_t& i32) = 0;
    virtual uint32_t readI64_virt(int64_t& i64) = 0;
    virtual uint32_t readString_virt(std::string& str) = 0;

    virtual uint32_t writeBool_virt(const bool value) = 0;
    virtual uint32_t writeByte_virt(const int8_t byte) = 0;
    virtual uint32_t writeI16_virt(const int16_t i16) = 0;
    virtual uint32_t writeI32_virt(const int32_t i32) = 0;
    virtual uint32_t writeI64_virt(const int64_t i64) = 0;
    virtual uint32_t writeString_virt(const std::string& str) = 0;

protected:
    IProtocol(std::shared_ptr<ITransport> pTransport):pTransport_(pTransport){}

    std::shared_ptr<ITransport> pTransport_;

private:
    IProtocol(){}
};

// 序列化继承的基类
class TBase {
public:
    virtual ~TBase(){};
    virtual uint32_t read(IProtocol* iprot) = 0;
    virtual uint32_t write(IProtocol* oprot) const = 0;
};

class BinaryProtocol : public IProtocol{
public:
    BinaryProtocol(std::shared_ptr<ITransport> pTransport):IProtocol(pTransport){}

    virtual uint32_t readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId);
    virtual uint32_t readFieldEnd();
    virtual uint32_t writeFieldBegin(const std::string& name, const TType fieldType, const int16_t fieldId);
    virtual uint32_t writeFieldEnd();

    virtual uint32_t readStructBegin(std::string& name);
    virtual uint32_t readStructEnd();

    virtual uint32_t readMessageBegin(std::string& name, TMessageType& messageType, int32_t& seqid);
    virtual uint32_t readMessageEnd();

    virtual uint32_t writeMessageBegin(const std::string& name,
                                       const TMessageType messageType,
                                       const int32_t seqid);

    virtual uint32_t writeMessageEnd();

    virtual uint32_t writeStructBegin(const char* name);

    virtual uint32_t writeStructEnd();

    virtual uint32_t writeFieldStop();

protected:
    virtual uint32_t readBool_virt(bool& value);
    virtual uint32_t readByte_virt(int8_t& byte);
    virtual uint32_t readI16_virt(int16_t& i16);
    virtual uint32_t readI32_virt(int32_t& i32);
    virtual uint32_t readI64_virt(int64_t& i64);
    virtual uint32_t readString_virt(std::string& str);

    virtual uint32_t writeBool_virt(const bool value);
    virtual uint32_t writeByte_virt(const int8_t byte);
    virtual uint32_t writeI16_virt(const int16_t i16);
    virtual uint32_t writeI32_virt(const int32_t i32);
    virtual uint32_t writeI64_virt(const int64_t i64);
    virtual uint32_t writeString_virt(const std::string& str);
};

class TProtocolFactory {
public:
    TProtocolFactory() {}

    virtual ~TProtocolFactory();

    virtual std::shared_ptr<IProtocol> getProtocol(std::shared_ptr<ITransport> trans) = 0;
    virtual std::shared_ptr<IProtocol> getProtocol(std::shared_ptr<ITransport> inTrans,
                                                   std::shared_ptr<ITransport> outTrans) {
        (void)outTrans;
        return getProtocol(inTrans);
    }
};

class TBinaryProtocolFactory : public TProtocolFactory{
    TBinaryProtocolFactory() {}

    virtual ~TBinaryProtocolFactory() {}

    virtual std::shared_ptr<IProtocol> getProtocol(std::shared_ptr<ITransport> trans){
        return std::shared_ptr<IProtocol>(new BinaryProtocol(trans));
    }
};

} // xy
