//
// Created by Administrator on 2023/6/21.
//

#pragma once
#include <iostream>

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

} // test
