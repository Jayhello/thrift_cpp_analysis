//
// Created by wenwen on 2023/6/17.
//

#pragma once

#include <cstdint>
#include <string>

namespace xy{

class IProtocol{
public:
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
};



} // xy
