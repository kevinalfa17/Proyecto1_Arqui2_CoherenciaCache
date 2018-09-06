/**
 * @brief Class that represent bus model
 * 
 * @file Bus.h
 * @author Kevin Alfaro 
 * @date 2018-09-05
 */

#ifndef BUS_H
#define BUS_H

#include <vector>
#include <iostream>
#include <string.h>
#include "BusMessage.h"
using namespace std;


class Bus{

private:
    int id;
    bool bus_clk;
    bool running;

    void broadcast(vector<bool> *snoop_flag);

public:
    Bus();
    ~Bus();

    void loop(bool clk, vector<BusMessage*> * queue, vector<bool> * snoop_flag, BusMessage * actualMessage, bool & bussy, bool & memoryEnable);
    void run();
    void stop();
    bool isRunning();

};

#endif