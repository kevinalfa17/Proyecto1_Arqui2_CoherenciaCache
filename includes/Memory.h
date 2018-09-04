#ifndef MEMORY_H
#define MEMORY_H

#include "BusMessage.h"
#include <vector>
#include <iostream>

using namespace std;

class Memory{

private:

    int blocks[16];
    bool running;
    bool memory_clk;
    
public:
    Memory();
    ~Memory();

    void loop(bool clk, bool & bussy, BusMessage * actualMessage, vector<bool> *snoop_flag, vector<BusMessage*> * queue);

    void printData();
    void broadcast(vector<bool> *snoop_flag);
    void run();
    void stop();
    bool isRunning();

};

#endif