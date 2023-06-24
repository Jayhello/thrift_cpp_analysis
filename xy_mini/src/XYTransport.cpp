//
// Created by wenwen on 2023/6/17.
//

#include "XYTransport.h"

namespace xy{

uint32_t TBufferedTransport::readSlow(uint8_t* buf, uint32_t len){
    setReadBuffer(rBuf_, transport_->read(rBuf_, rBufSize_));

    // Hand over whatever we have.
    uint32_t give = (std::min)(len, static_cast<uint32_t>(rBound_ - rBase_));
    memcpy(buf, rBase_, give);
    rBase_ += give;

    return give;
}

void TBufferedTransport::writeSlow(const uint8_t* buf, uint32_t len){
    transport_->write(buf, len);
    wBase_ = wBuf_;
}

void TBufferedTransport::flush(){

}

TBufferedTransport::~TBufferedTransport(){
    if(nullptr != rBuf_){
        delete [] rBuf_;
        rBufSize_ = 0;
    }
    if(nullptr == wBuf_){
        delete[] wBuf_;
        wBufSize_ = 0;
    }
}

uint32_t LocalTransport::read(uint8_t* buf, uint32_t len){
    uint32_t have = buf_.size();
    if(0 == have) return 0;
    uint32_t k = std::min(have, len);
    std::memcpy(buf, buf_.data(), k);
    buf_.erase(buf_.begin(), buf_.begin() + k);
    return k;
}

void LocalTransport::write(const uint8_t* buf, uint32_t len){
    for(uint32_t i = 0; i < len; ++i){
        buf_.push_back(buf[i]);
    }
}

} // xy
