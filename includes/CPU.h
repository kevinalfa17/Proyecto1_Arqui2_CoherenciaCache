/**
 * @brief CPU model
 * 
 * @file CPU.h
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "BusMessage.h"

using namespace std;

class CPU{

private:
    enum State {PROCESSING,READING,WRITING,FREE};
    State cpu_state;
    int id;
    int process_timer;
    bool cpu_clk;

    void request();
    void process();
    void write(int & data, int & address, bool & write_flag, bool & ready);
    void read(int & data, int & address, bool & read_flag, bool & ready);

public:
    
    CPU(int id);
    ~CPU();  

    void loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & ready, bool & controlEnable);
    int getId();
    void getState();
    
};

#endif