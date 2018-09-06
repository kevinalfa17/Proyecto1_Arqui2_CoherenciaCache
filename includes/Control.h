/**
 * @brief Control unit model
 * 
 * @file Control.h
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "BusMessage.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Control{
       
public:
    enum State{INVALID,MODIFIED,SHARED,OWNER,EXCLUSIVE};

    Control(int id);
    ~Control();

    void initStates();
    State getState(int block_number);
    void setState(int block_number,State state);
    string getStateString(int block_number);
    vector<string> getStates();
    
    void loop(bool clk,int & data, int & address, bool & write_flag_cpu, bool & read_flag_cpu, bool & write_flag_cache, bool & read_flag_cache, bool & ready, vector<bool> * snoop_flag,  vector<BusMessage*> * queue, BusMessage * actualMessage, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache, bool & busEnable, bool & cacheEnable);
    int getId();

private:
    int id;
    bool control_clk;
    bool wait;
    vector<State> * states;
};

#endif