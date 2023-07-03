//
// Created by wenwen on 2023/6/17.
//

#include <iostream>
#include <memory>
#include "XYTransport.h"
#include "xy_example.h"
#include "xy_servertransport.h"

int main(int argc, char** argv){

//    test::test_template_virtual();
//    test::test_template_virtual2();
//    test::test_local_transport1();

    test_svr::test_server_transport();

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

} // test_svr
