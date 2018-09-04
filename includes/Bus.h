#ifndef BUS_H
#define BUS_H

#include <vector>
#include <iostream>
#include "BusMessage.h"
using namespace std;



class Bus{

private:
    int id;
    bool bussy; 
    bool bus_clk;
    int handler;
    bool running;

    void broadcast(vector<bool> *snoop_flag);

public:
    Bus();
    ~Bus();

    void loop(bool clk, vector<BusMessage*> * queue, vector<bool> * snoop_flag, BusMessage * actualMessage);
    void run();
    void stop();
    bool isRunning();

};

#endif