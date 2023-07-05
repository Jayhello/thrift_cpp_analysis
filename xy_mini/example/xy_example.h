//
// Created by Administrator on 2023/6/21.
//

#pragma once
#include <iostream>
#include <functional>
#include <map>
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

