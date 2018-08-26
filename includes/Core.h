#ifndef CORE_H
#define CORE_H

#include "CPU.h"

class Core{

private:

    int id;
    int data;
    int address;
    bool ready;
    bool read_flag;
    bool write_flag;
    bool running;

    CPU * cpu;
    
    
public:

    Core(int id);
    ~Core();
    void update(bool clk);
    void run();
    void stop();
    bool isRunning();

};

#endif