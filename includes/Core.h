#ifndef CORE_H
#define CORE_H

#include "CPU.h"
#include "Control.h"
#include "Cache.h"
#include "BusMessage.h"
#include <vector>

using namespace std;

class Core{

private:

    int id;
    bool running;

    //Conections
    
    int data;
    int address;
    int snoop_data_cache;
    int snoop_address_cache;
    bool ready;
    bool read_flag_cpu;
    bool write_flag_cpu;
    bool read_flag_cache;
    bool write_flag_cache;  
    bool snoop_flag_cache;

    CPU * cpu;
    Control * control;
    Cache * cache;
    
    
public:

    Core(int id);
    ~Core();
    void cpu_loop(bool clk, bool & controlEnable);
    void cache_loop(bool clk);
    void control_loop(bool clk, vector<BusMessage*> * queue, vector<bool> * snoop_flag, BusMessage* actual_message, bool & busEnable, bool & cacheEnable);
    void run();
    void stop();
    bool isRunning();
    int getId();
    void printCache();

};

#endif