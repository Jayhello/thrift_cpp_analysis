//
// Created by wenwen on 2023/6/17.
//

#include "XYProtocol.h"

namespace xy{

uint32_t BinaryProtocol::readFieldBegin(std::string& name, TType& fieldType, int16_t& fieldId){

}

uint32_t BinaryProtocol::readFieldEnd(){

}

uint32_t BinaryProtocol::writeFieldBegin(const std::string& name, const TType fieldType, const int16_t fieldId){

}

uint32_t BinaryProtocol::writeFieldEnd(){

}

uint32_t BinaryProtocol::readBool_virt(bool& value){

}

uint32_t BinaryProtocol::readByte_virt(int8_t& byte){

}

uint32_t BinaryProtocol::readI16_virt(int16_t& i16){

}

uint32_t BinaryProtocol::readI32_virt(int32_t& i32){

}

uint32_t BinaryProtocol::readI64_virt(int64_t& i64){

}

uint32_t BinaryProtocol::readString_virt(std::string& str){

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
