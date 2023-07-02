//
// Created by wenwen on 2023/7/2.
//

#pragma once
#include <thread>

namespace xy{

class Runnable{
public:
    virtual ~Runnable(){};
    virtual void run() = 0;

    // todo add std::thread
};

} // xy
