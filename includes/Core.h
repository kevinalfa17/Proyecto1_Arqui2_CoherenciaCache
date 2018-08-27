#ifndef CORE_H
#define CORE_H

#include "CPU.h"

class Core{

private:

    int id;
    bool running;

    //Conections
    bool ready;
    int data_cpu;
    int address_cpu;
    int data_cache;
    int address_cache;
    bool control_clk;
    bool read_flag_cpu;
    bool write_flag_cpu;
    bool read_flag_cache;
    bool write_flag_cache;
    bool snoop_flag_cache;
    bool snoop_flag_bus;
    
    //Components
    CPU * cpu;
    
    
public:

    Core(int id);
    ~Core();
    void cpu_loop(bool clk);
    void cache_loop(bool clk);
    void run();
    void stop();
    bool isRunning();

};

#endif