//
// Created by wenwen on 2023/6/17.
//

#include "XYProtocol.h"

namespace xy{

uint32_t BinaryProtocol::readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId){
    uint32_t result = 0;
    int8_t type;
    result += readByte(type);
    fieldType = (TType)type;
    if(T_STOP == fieldType){   // stop的就没有 id 了
        fieldId = 0;
        return result;
    }
    result += readI16(fieldId);
    return result;
}

uint32_t BinaryProtocol::writeFieldStop(){
    return writeByte((int8_t)T_STOP);
}

uint32_t BinaryProtocol::readFieldEnd(){
    return 0;
}

uint32_t BinaryProtocol::writeFieldBegin(const std::string& name, const TType fieldType, const int16_t fieldId){
    uint32_t wsize = 0;
    wsize += writeByte((int8_t)fieldType);
    wsize += writeI16(fieldId);
    return wsize;
}

uint32_t BinaryProtocol::writeFieldEnd(){
    return 0;
}


uint32_t BinaryProtocol::readStructBegin(std::string& name){
    return 0;
}


uint32_t BinaryProtocol::readStructEnd(){
    return 0;
}

uint32_t BinaryProtocol::readMessageBegin(std::string& name, TMessageType& messageType, int32_t& seqid){
    uint32_t result = 0;

    int8_t type;
    result += readString(name);
    result += readByte(type);
    messageType = (TMessageType)type;
    result += readI32(seqid);
    return result;
}

uint32_t BinaryProtocol::readMessageEnd(){
    return 0;
}


uint32_t BinaryProtocol::writeMessageBegin(const std::string& name,
                                   const TMessageType messageType,
                                   const int32_t seqid){
    uint32_t wsize = 0;
    wsize += writeString(name);
    wsize += writeByte((int8_t)messageType);
    wsize += writeI32(seqid);
    return wsize;
}

uint32_t BinaryProtocol::writeMessageEnd(){
    return 0;
}

uint32_t BinaryProtocol::writeStructBegin(const char* name){
    return 0;
}

uint32_t BinaryProtocol::writeStructEnd(){
    return 0;
}

uint32_t BinaryProtocol::readBool_virt(bool& value){
    uint8_t v = 1;
    pTransport_->read(&v, 1);
    value = v;
    return 1;
}

uint32_t BinaryProtocol::readByte_virt(int8_t& byte){
    uint8_t b[1];
    pTransport_->readAll(b, 1);
    byte = *(int8_t*)b;
    return 1;
}

uint32_t BinaryProtocol::readI16_virt(int16_t& i16){
    union bytes {
        uint8_t b[2];
        int16_t all;
    } theBytes;
    pTransport_->readAll(theBytes.b, 2);
//    i16 = (int16_t)ByteOrder_::fromWire16(theBytes.all);
    i16 = theBytes.all;
    return 2;
}

uint32_t BinaryProtocol::readI32_virt(int32_t& i32){
    union bytes {
        uint8_t b[4];
        int32_t all;
    } theBytes;
    pTransport_->readAll(theBytes.b, 4);
//    i32 = (int32_t)ByteOrder_::fromWire32(theBytes.all);
    i32 = theBytes.all;
    return 4;
}

uint32_t BinaryProtocol::readI64_virt(int64_t& i64){
    union bytes {
        uint8_t b[8];
        int64_t all;
    } theBytes;
    pTransport_->readAll(theBytes.b, 8);
//    i64 = (int64_t)ByteOrder_::fromWire64(theBytes.all);
    i64 = theBytes.all;
    return 8;
}

uint32_t BinaryProtocol::readString_virt(std::string& str){
    uint32_t result;
    int32_t size;
    result = readI32(size);
    str.resize(size);
    pTransport_->readAll(reinterpret_cast<uint8_t*>(&str[0]), size);
    return result + (uint32_t)size;
}

uint32_t BinaryProtocol::writeBool_virt(const bool value){
    uint8_t v = value;
    pTransport_->write(&v, 1);
    return 1;
}

uint32_t BinaryProtocol::writeByte_virt(const int8_t byte){
    pTransport_->write((uint8_t*)&byte, 1);
    return 1;
}

uint32_t BinaryProtocol::writeI16_virt(const int16_t i16){
//    int16_t net = (int16_t)ByteOrder_::toWire16(i16);
    pTransport_->write((uint8_t*)&i16, 2);
    return 2;
}

uint32_t BinaryProtocol::writeI32_virt(const int32_t i32){
    pTransport_->write((uint8_t*)&i32, 4);
    return 4;
}

uint32_t BinaryProtocol::writeI64_virt(const int64_t i64){
    pTransport_->write((uint8_t*)&i64, 8);
    return 8;
}

uint32_t BinaryProtocol::writeString_virt(const std::string& str){
    uint32_t len = str.size();
    uint32_t result = writeI32((int32_t)len);
    if (len > 0) {
        pTransport_->write((uint8_t*)str.data(), len);
    }
    return result + len;
}

} // xy
