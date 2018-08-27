#ifndef CONTROL_H
#define CONTROL_H

#include "CacheBlock.h"
#include <iostream>

using namespace std;

class Control{

private:
    int id;
    bool control_clk;
    
    
public:
    Control(int id);
    ~Control();

    void loop(bool clk,int & data_cpu, int & address_cpu, bool & write_flag_cpu, bool & read_flag_cpu,int & data_cache, int & address_cache, bool & write_flag_cache, bool & read_flag_cache, bool & snoop_flag_cache, bool & snoop_flag_bus, CacheBlock::State & state);
    int getId();
};

#endif