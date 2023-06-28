//
// Created by wenwen on 2023/6/17.
//

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <cstring>

namespace xy{

template <class Transport_>
uint32_t readAll(Transport_& trans, uint8_t* buf, uint32_t len) {
    uint32_t have = 0;
    uint32_t get = 0;

    while (have < len) {
        get = trans.read(buf + have, len - have);
        if (get <= 0) {
            break;
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

    virtual uint32_t read(uint8_t* buf, uint32_t len) = 0;

    virtual uint32_t readAll(uint8_t* buf, uint32_t len){
        return xy::readAll(*this, buf, len);
    }

    // Writes the string in its entirety to the buffer.
    // Note: You must call flush() to ensure the data is actually written,
    virtual void write(const uint8_t* buf, uint32_t len) = 0;

    virtual uint32_t readEnd() {
        // default behaviour is to do nothing
        return 0;
    }

    virtual uint32_t writeEnd() {
        // default behaviour is to do nothing
        return 0;
    }

    // Flushes any pending data to be written. Typically used with buffered
    virtual void flush() {
        // default behaviour is to do nothing
    }

    /*------------  下面这些函数我觉得也没有必要设计的 virtual 然后里面有加一堆包装, 看着很蛋疼 ------------*/
//    const uint8_t* borrow(uint8_t* buf, uint32_t* len) {
//        return borrow_virt(buf, len);
//    }

//    virtual const uint8_t* borrow_virt(uint8_t* /* buf */, uint32_t* /* len */) { return nullptr; }

//    void consume(uint32_t len) {
//        consume_virt(len);
//    }

//    virtual void consume_virt(uint32_t /* len */) {
//        throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot consume.");
//    }

//    uint32_t read(uint8_t* buf, uint32_t len) {
//        return read_virt(buf, len);
//    }

//    virtual uint32_t read_virt(uint8_t* /* buf */, uint32_t /* len */) {
//        throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot read.");
//    }

//    uint32_t readAll(uint8_t* buf, uint32_t len) {
//        return readAll_virt(buf, len);
//    }
//
//    virtual uint32_t readAll_virt(uint8_t* buf, uint32_t len) {
//        return xy::readAll(*this, buf, len);
//    }

protected:
    ITransport() = default;
};

class TBufferBase : public ITransport{
public:
    virtual uint32_t read(uint8_t* buf, uint32_t len){
        uint8_t* newBase = rBase_ + len;
        if(newBase <= rBound_){
            std::memcpy(buf, rBound_, len);
            rBound_ = newBase;
            return len;
        }
        return readSlow(buf, len);
    }

    virtual void write(const uint8_t* buf, uint32_t len){
        uint8_t* newWBase = wBase_ + len;
        if(newWBase < wBound_){
            std::memcpy(wBase_, buf, len);
            wBase_ = newWBase;
            return;
        }
        writeSlow(buf, len);
    }

protected:
    virtual uint32_t readSlow(uint8_t* buf, uint32_t len) = 0;

    virtual void writeSlow(const uint8_t* buf, uint32_t len) = 0;

    void setReadBuffer(uint8_t* buf, uint32_t len){
        rBase_ = buf;
        rBound_ = rBase_ + len;
    }

    void setWriteBuffer(uint8_t* buf, uint32_t len){
        wBase_ = buf;
        wBound_ = wBase_ + len;
    }

    TBufferBase() = default;
    virtual ~TBufferBase(){}

protected:

    uint8_t* rBase_ = nullptr;
    uint8_t* rBound_ = nullptr;
    uint8_t* wBase_ = nullptr;
    uint8_t* wBound_ = nullptr;
};

class TBufferedTransport : public TBufferBase{
public:
    static const int DEFAULT_BUFFER_SIZE = 512;

    virtual void open() { transport_->open(); }

    virtual bool isOpen() { return transport_->isOpen(); }

    bool peek() {
        if (rBase_ == rBound_) {
            setReadBuffer(rBuf_, transport_->read(rBuf_, rBufSize_));
        }
        return (rBound_ > rBase_);
    }

    void close() {
        flush();
        transport_->close();
    }

    void flush();

    TBufferedTransport(std::shared_ptr<ITransport> pTransport):
        transport_(pTransport), rBufSize_(512), wBufSize_(512),
        rBuf_(new uint8_t[rBufSize_]),
        wBuf_(new uint8_t[wBufSize_]){
            initPointers();
        }

    virtual ~TBufferedTransport();

protected:
    virtual uint32_t readSlow(uint8_t* buf, uint32_t len);

    virtual void writeSlow(const uint8_t* buf, uint32_t len);

    void initPointers(){
        setReadBuffer(rBuf_, 0);
        setWriteBuffer(wBuf_, wBufSize_);
    }

protected:
    std::shared_ptr<ITransport> transport_;

    uint32_t rBufSize_;
    uint32_t wBufSize_;
    uint8_t* rBuf_;
    uint8_t* wBuf_;
};

// 内存储存传输用于测试protocol的序列化与反序列化(必须先write后才能read)
class LocalTransport : public ITransport{
public:
    virtual bool isOpen() { return true; }
    // Tests whether there is more data to read or if the remote side is still open.
    virtual bool peek() { return isOpen(); }
    // Opens the transport for communications.
    virtual void open() {
    }
    virtual void close() {
    }

    virtual uint32_t read(uint8_t* buf, uint32_t len);

    // Writes the string in its entirety to the buffer.
    // Note: You must call flush() to ensure the data is actually written,
    virtual void write(const uint8_t* buf, uint32_t len);

private:
    std::vector<uint8_t> buf_;
};


} // xy
