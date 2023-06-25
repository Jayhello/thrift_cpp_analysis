//
// Created by wenwen on 2023/6/17.
//

#include <iostream>
#include <memory>
#include "XYProtocol.h"
#include "XYTransport.h"
#include "xy_example.h"

int main(int argc, char** argv){

//    test::test_template_virtual();
    test::test_template_virtual2();

    std::cout << "123" << std::endl;
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

}

}