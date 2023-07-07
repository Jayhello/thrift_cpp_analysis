//
// Created by Administrator on 2023/6/21.
//

#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <thread>
#include "XYProtocol.h"
#include "xy_processor.h"

namespace test{

// 验证下模板的虚函数(下面这种继承方式以及模板的静态转化, 不太好理解, 而且容易出问题)
class ITest{
public:
    virtual void read() = 0;

    virtual void write() = 0;
};

class TestDefault : public ITest{
public:
    virtual void read(){
        std::cout << "default_read" << std::endl;
    }

    virtual void write(){
        std::cout << "default_write" << std::endl;
    }
};

template<typename Test_, class Supper_ = TestDefault>
class VirtualTest : public Supper_{
public:
    virtual void read(){
        static_cast<Test_*>(this)->read();
    }

    virtual void write(){
        static_cast<Test_*>(this)->write();
    }
};

class Test1 : public VirtualTest<Test1>{
public:
    virtual void write(){
        std::cout << "test1's write" << std::endl;
    }
};

class Test2 : public TestDefault{
public:
    virtual void write(){
        std::cout << "test2's write" << std::endl;
    }
};

void test_template_virtual();

void test_template_virtual2();

void test_local_transport1();

void test_exception_proto();

class TestProto : public xy::TBase{
public:
    TestProto(int v=0, bool b=false, const std::string& s = ""):iVal(v), bFlag(b), str(s){}

    virtual uint32_t read(xy::IProtocol* iprot);
    virtual uint32_t write(xy::IProtocol* oprot)const;

    virtual void printTo(std::ostream& out) const;

    int32_t iVal;
    bool bFlag;
    std::string str;
};

std::ostream& operator<<(std::ostream& out, const TestProto& obj);

} // test

//
namespace test_svr{

void test_server_transport();

class TestReq : public xy::TBase{
public:
    TestReq(int v=0, bool b=false, const std::string& s = ""):iVal(v), bFlag(b), str(s){}

    virtual uint32_t read(xy::IProtocol* iprot);
    virtual uint32_t write(xy::IProtocol* oprot)const;

    virtual void printTo(std::ostream& out) const;

    int32_t iVal;
    bool bFlag;
    std::string str;
};

class TestInt_args{
public:
    virtual uint32_t read(xy::IProtocol* iprot);
    virtual uint32_t write(xy::IProtocol* oprot)const;

    int a;
};

class TestInt_return{
public:

    int success;
};

class TestStr_args{
public:

    std::string str;
};

class TestStr_return{
public:

    int success;
};

class TestIf{
public:
    virtual int testInt(int) = 0;

    virtual int testStr(const std::string& ) = 0;
};

class TestProcessor : public xy::TDispatchProcessor{
public:
//    using ProcessFunction = std::function<bool(int32_t, xy::IProtocol*, xy::IProtocol*, void*)>;  // 这种定义方式不行
    typedef bool(TestProcessor::*ProcessFunction)(int32_t, xy::IProtocol*, xy::IProtocol*, void*);

    using ProcessMap = std::map<std::string, ProcessFunction>;

    TestProcessor(const std::shared_ptr<TestIf>& iface):iface_(iface){
        processMap_["testInt"] = &TestProcessor::process_testInt;
        processMap_["testStr"] = &TestProcessor::process_testStr;
    }
private:
    bool process_testInt(int32_t seqid, xy::IProtocol* in, xy::IProtocol* out, void* callContext);

    bool process_testStr(int32_t seqid, xy::IProtocol* in, xy::IProtocol* out, void* callContext);

protected:
    virtual bool dispatchCall(xy::IProtocol* in,
                              xy::IProtocol* out,
                              const std::string& fname,
                              int32_t seqid,
                              void* callContext);
    std::shared_ptr<TestIf> iface_;
    ProcessMap              processMap_;
};


} // test_svr


// 线程相关的测试
namespace test_thread{

void test_thread();

class Thread;

class Runnable {
public:
    virtual ~Runnable(){};
    virtual void run() = 0;

    virtual std::shared_ptr<Thread> thread() { return thread_.lock(); }
    virtual void thread(std::shared_ptr<Thread> value) { thread_ = value; }

private:
    std::weak_ptr<Thread> thread_;
};

class Thread {
public:
    typedef std::thread::id id_t;
    static inline bool is_current(id_t t) { return t == std::this_thread::get_id(); }
    static inline id_t get_current() { return std::this_thread::get_id(); }

    virtual ~Thread(){};

    virtual void start() = 0;

    virtual void join() = 0;

    // Gets the thread's platform-specific ID
    virtual id_t getId() = 0;

    // Gets the runnable object this thread is hosting
    virtual std::shared_ptr<Runnable> runnable() const { return _runnable; }

protected:
    virtual void runnable(std::shared_ptr<Runnable> value) { _runnable = value; }

private:
    std::shared_ptr<Runnable> _runnable;
};

class StdThread : public Thread, public std::enable_shared_from_this<StdThread> {
public:
    enum STATE { uninitialized, starting, started, stopping, stopped };

    static void threadMain(std::shared_ptr<StdThread> thread);

    StdThread(bool detached, std::shared_ptr<Runnable> runnable):state_(uninitialized),detached_(detached){
        this->Thread::runnable(runnable);
    }

    ~StdThread();

    STATE getState() const;

    void setState(STATE newState);

    void start();

    void join();

    Thread::id_t getId() { return thread_.get() ? thread_->get_id() : std::thread::id(); }

    std::shared_ptr<Runnable> runnable() const { return Thread::runnable(); }

    void runnable(std::shared_ptr<Runnable> value) { Thread::runnable(value); }

private:
    std::unique_ptr<std::thread> thread_;
    //    Monitor monitor_;
    STATE state_;
    bool detached_;
};


// Factory to create platform-specific thread object and bind them to Runnable object for execution
class ThreadFactory {
protected:
    ThreadFactory(bool detached) : detached_(detached) { }
public:
    virtual ~ThreadFactory() {}

    bool isDetached() const { return detached_; }
    void setDetached(bool detached) { detached_ = detached; }

    virtual std::shared_ptr<Thread> newThread(std::shared_ptr<Runnable> runnable) const = 0;
    virtual Thread::id_t getCurrentThreadId() const = 0;
    static const Thread::id_t unknown_thread_id;
private:
    bool detached_;
};

class StdThreadFactory : public ThreadFactory {
public:
    StdThreadFactory(bool detached = true);

    std::shared_ptr<Thread> newThread(std::shared_ptr<Runnable> runnable) const;

    Thread::id_t getCurrentThreadId() const;
};


} // test_thread
