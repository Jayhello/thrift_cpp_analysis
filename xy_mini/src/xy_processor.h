//
// Created by wenwen on 2023/7/2.
//
#pragma once
#include "XYProtocol.h"
#include "XYTransport.h"

namespace xy{

class TProcessor {
public:
    virtual ~TProcessor() {}

    virtual bool process(std::shared_ptr<IProtocol> in,
                         std::shared_ptr<IProtocol> out,
                         void* connectionContext) = 0;

    bool process(std::shared_ptr<IProtocol> io, void* connectionContext) {
        return process(io, io, connectionContext);
    }

    /*
    std::shared_ptr<TProcessorEventHandler> getEventHandler() const { return eventHandler_; }
    void setEventHandler(std::shared_ptr<TProcessorEventHandler> eventHandler) {
        eventHandler_ = eventHandler;
    }
    */
protected:
    TProcessor() {}

//    std::shared_ptr<TProcessorEventHandler> eventHandler_;
};

struct TConnectionInfo {
    // The input and output protocols
    std::shared_ptr<IProtocol> input;
    std::shared_ptr<IProtocol> output;
    // The underlying transport used for the connection
    // This is the transport that was returned by TServerTransport::accept(),
    // and it may be different than the transport pointed to by the input and
    // output protocols.
    std::shared_ptr<ITransport> transport;
};


class TProcessorFactory {
public:
    virtual ~TProcessorFactory() {}
    virtual std::shared_ptr<TProcessor> getProcessor(const TConnectionInfo& connInfo) = 0;
    virtual std::shared_ptr<TProcessor> getProcessor() = 0;
};

class TSingletonProcessorFactory : public TProcessorFactory {
public:
    TSingletonProcessorFactory(std::shared_ptr<TProcessor> processor) : processor_(processor) {}
    std::shared_ptr<TProcessor> getProcessor(const TConnectionInfo&) { return processor_; }

    std::shared_ptr<TProcessor> getProcessor() { return processor_; }

private:
    std::shared_ptr<TProcessor> processor_;
};

class TDispatchProcessor : public TProcessor {
public:
    virtual bool process(std::shared_ptr<IProtocol> in,
                         std::shared_ptr<IProtocol> out,
                         void* connectionContext) {
        std::string fname;
        TMessageType mtype;
        int32_t seqid;
        in->readMessageBegin(fname, mtype, seqid);

        if (mtype != T_CALL && mtype != T_ONEWAY) {
//            GlobalOutput.printf("received invalid message type %d from client", mtype);
            return false;
        }

        return dispatchCall(in.get(), out.get(), fname, seqid, connectionContext);
    }

protected:
    virtual bool dispatchCall(IProtocol* in,
                              IProtocol* out,
                              const std::string& fname,
                              int32_t seqid,
                              void* callContext) = 0;
};

} // xy
