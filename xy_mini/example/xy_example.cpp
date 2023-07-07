//
// Created by wenwen on 2023/6/17.
//

#include <iostream>
#include <memory>
#include "XYTransport.h"
#include "xy_example.h"
#include "logging.h"
#include "xy_servertransport.h"

int main(int argc, char** argv){

//    test::test_template_virtual();
//    test::test_template_virtual2();
//    test::test_local_transport1();
//    test::test_exception_proto();

//    test_svr::test_server_transport();

    test_thread::test_thread();

    std::cout << "example end" << std::endl;
    return 0;
}

namespace test{

// 下面这种继承方式以及模板的静态转化, 不太好理解, 而且容易出问题(直接core dump了)
void test_template_virtual(){

    ITest* pt = new Test1();
    pt->read();
    pt->write();
}

// 下面不会core
void test_template_virtual2(){
    ITest* pt = new Test2();
    pt->read();
    pt->write();
}

void test_local_transport1(){
    std::shared_ptr<xy::ITransport> local(new xy::LocalTransport());
    std::shared_ptr<xy::ITransport> transport(new xy::TBufferedTransport(local));

    std::shared_ptr<xy::IProtocol> protocol(new xy::BinaryProtocol(transport));

    TestProto tp1(123, true, "abc");
    uint32_t size = tp1.write(protocol.get());
    std::cout << "write_size: " << size << ", tp1: " << tp1 << std::endl;

    transport->flush();

    TestProto tp2;
    size = tp2.read(protocol.get());
    std::cout << "read_size: " << size << ", tp2: " << tp2 << std::endl;
}

void test_exception_proto(){
    std::shared_ptr<xy::ITransport> local(new xy::LocalTransport());
    std::shared_ptr<xy::ITransport> transport(new xy::TBufferedTransport(local));
    std::shared_ptr<xy::IProtocol> protocol(new xy::BinaryProtocol(transport));

    xy::TApplicationException ex1(xy::TApplicationException::PROTOCOL_ERROR, "test_1234");
    uint32_t size = ex1.write(protocol.get());
    std::cout << "write_size: " << size << ", ex1: " << ex1 << std::endl;

    transport->flush();

    xy::TApplicationException ex2;
    size = ex2.read(protocol.get());
    std::cout << "read_size: " << size << ", ex2: " << ex2 << std::endl;
}

uint32_t TestProto::read(xy::IProtocol* iprot){
    uint32_t xfer = 0;
    std::string fname;
    xy::TType ftype;
    int16_t fid;

    xfer += iprot->readFieldBegin(fname, ftype, fid);
    xfer += iprot->readI32(iVal);
    xfer += iprot->readFieldEnd();

    xfer += iprot->readFieldBegin(fname, ftype, fid);
    xfer += iprot->readBool(bFlag);
    xfer += iprot->readFieldEnd();

    xfer += iprot->readFieldBegin(fname, ftype, fid);
    xfer += iprot->readString(str);
    xfer += iprot->readFieldEnd();

    return xfer;
}

uint32_t TestProto::write(xy::IProtocol* oprot)const{
    uint32_t xfer = 0;

    xfer += oprot->writeFieldBegin("val", xy::T_I32, 1);
    xfer += oprot->writeI32(iVal);
    xfer += oprot->writeFieldEnd();

    xfer += oprot->writeFieldBegin("flag", xy::T_BOOL, 2);
    xfer += oprot->writeBool(bFlag);
    xfer += oprot->writeFieldEnd();

    xfer += oprot->writeFieldBegin("str", xy::T_STRING, 3);
    xfer += oprot->writeString(str);
    xfer += oprot->writeFieldEnd();

    return xfer;
}

void TestProto::printTo(std::ostream& out) const{
    out << "TestProto(";
    out << "iVal=" << std::to_string(iVal);
    out << ", " << "bFlag=" << std::to_string(bFlag);
    out << ", " << "str=" << str;
    out << ")";
}

std::ostream& operator<<(std::ostream& out, const TestProto& obj){
    obj.printTo(out);
    return out;
}

} // test

namespace test_svr{

void test_server_transport(){
    xy::TServerSocket ts(8889);
    ts.listen();
}


uint32_t TestInt_args::read(xy::IProtocol* iprot){

}

uint32_t TestInt_args::write(xy::IProtocol* oprot)const{

}

bool TestProcessor::process_testInt(int32_t seqid, xy::IProtocol* in, xy::IProtocol* out, void* callContext){
    try{

    }catch(const std::exception& ex){

    }

    return 0;
}

bool TestProcessor::process_testStr(int32_t seqid, xy::IProtocol* in, xy::IProtocol* out, void* callContext){
    return 0;
}

bool TestProcessor::dispatchCall(xy::IProtocol* iprot, xy::IProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext){
    auto it = processMap_.find(fname);
    if(it == processMap_.end()){

        return true;
    }

    (this->*(it->second))(seqid, iprot, oprot, callContext);
    return true;
}


} // test_svr

namespace test_thread{
using xy::Logger;

class Run1: public Runnable{
public:
    virtual void run(){
        info("run1 starting...");
//        std::cout << "run1 starting..." << std::endl;
        std::chrono::seconds sec(1);
        std::this_thread::sleep_for(sec);
        info("run1 end");
//        std::cout << "run1 end" << std::endl;
    }
};

class Run2: public Runnable{
public:
    virtual void run(){
        info("run2 starting...");
        //        std::cout << "run1 starting..." << std::endl;
        std::chrono::seconds sec(2);
        std::this_thread::sleep_for(sec);
        info("run2 end");
    }
};

void test_thread(){
    StdThreadFactory factory(false);
    auto th1 = factory.newThread(std::shared_ptr<Run1>(new Run1));
    auto th2 = factory.newThread(std::shared_ptr<Run2>(new Run2));

    th1->start();
    th2->start();

    th1->join();
    th2->join();
}

void StdThread::threadMain(std::shared_ptr<StdThread> thread){
    thread->setState(started);
    thread->runnable()->run();

    if (thread->getState() != stopping && thread->getState() != stopped) {
        thread->setState(stopping);
    }
}

StdThread::~StdThread(){
    if (!detached_ && thread_->joinable()) {
        try {
            join();
        } catch (...) {
            // We're really hosed.
        }
    }
}


StdThread::STATE StdThread::getState() const{
//    Synchronized sync(monitor_);
    return state_;
}


void StdThread::setState(StdThread::STATE newState){
//    Synchronized sync(monitor_);
    state_ = newState;

    // unblock start() with the knowledge that the thread has actually
    // started running, which avoids a race in detached threads.
//    if (newState == started) {
//        monitor_.notify();
//    }
}


void StdThread::start(){
    if (getState() != uninitialized) {
        return;
    }

    std::shared_ptr<StdThread> selfRef = shared_from_this();
    setState(starting);

//    Synchronized sync(monitor_);
    thread_ = std::unique_ptr<std::thread>(new std::thread(threadMain, selfRef));

    if (detached_)
        thread_->detach();

    // Wait for the thread to start and get far enough to grab everything
    // that it needs from the calling context, thus absolving the caller
    // from being required to hold on to runnable indefinitely.
//    monitor_.wait();
}


void StdThread::join(){
    if (!detached_ && state_ != uninitialized) {
        thread_->join();
    }
}

StdThreadFactory::StdThreadFactory(bool detached):ThreadFactory(detached){
}

std::shared_ptr<Thread> StdThreadFactory::newThread(std::shared_ptr<Runnable> runnable) const{
    std::shared_ptr<StdThread> result = std::shared_ptr<StdThread>(new StdThread(isDetached(), runnable));
    runnable->thread(result);
    return result;
}


Thread::id_t StdThreadFactory::getCurrentThreadId() const{
    return std::this_thread::get_id();
}


} // test_thread
