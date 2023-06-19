//
// Created by wenwen on 2023/6/17.
//

#pragma once

#include <cstdint>

namespace xy{

template <class Transport_>
uint32_t readAll(Transport_& trans, uint8_t* buf, uint32_t len) {
    uint32_t have = 0;
    uint32_t get = 0;

    while (have < len) {
        get = trans.read(buf + have, len - have);
        if (get <= 0) {
//            throw TTransportException(TTransportException::END_OF_FILE, "No more data to read.");
        }
        have += get;
    }

    return have;
}

class ITransport{
public:
    virtual ~ITransport() {}

    // Whether this transport is open.
    virtual bool isOpen() { return false; }

    // Tests whether there is more data to read or if the remote side is still open.
    virtual bool peek() { return isOpen(); }

    // Opens the transport for communications.
    virtual void open() {
//        throw TTransportException(TTransportException::NOT_OPEN, "Cannot open base TTransport.");
    }

    virtual void close() {
//        throw TTransportException(TTransportException::NOT_OPEN, "Cannot close base TTransport.");
    }

    uint32_t read(uint8_t* buf, uint32_t len) {
        return read_virt(buf, len);
    }

    virtual uint32_t read_virt(uint8_t* /* buf */, uint32_t /* len */) {
//        throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot read.");
    }

    uint32_t readAll(uint8_t* buf, uint32_t len) {
        return readAll_virt(buf, len);
    }

    virtual uint32_t readAll_virt(uint8_t* buf, uint32_t len) {
        return xy::readAll(*this, buf, len);
    }

    virtual uint32_t readEnd() {
        // default behaviour is to do nothing
        return 0;
    }

    // Writes the string in its entirety to the buffer.
    // Note: You must call flush() to ensure the data is actually written,
    void write(const uint8_t* buf, uint32_t len) {
        write_virt(buf, len);
    }
    virtual void write_virt(const uint8_t* /* buf */, uint32_t /* len */) {
//        throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot write.");
    }

    virtual uint32_t writeEnd() {
        // default behaviour is to do nothing
        return 0;
    }

    // Flushes any pending data to be written. Typically used with buffered
    virtual void flush() {
        // default behaviour is to do nothing
    }

    const uint8_t* borrow(uint8_t* buf, uint32_t* len) {
        return borrow_virt(buf, len);
    }

    virtual const uint8_t* borrow_virt(uint8_t* /* buf */, uint32_t* /* len */) { return nullptr; }

    void consume(uint32_t len) {
        consume_virt(len);
    }

    virtual void consume_virt(uint32_t /* len */) {
//        throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot consume.");
    }

protected:
    ITransport() {}
};

class TBufferBase : public ITransport{
public:
    TBufferBase() = default;
};

} // xy
