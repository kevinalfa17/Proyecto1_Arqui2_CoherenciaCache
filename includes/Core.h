#ifndef CORE_H
#define CORE_H

#include "CPU.h"

class Core{

private:

    CPU cpu;
    bool ready;
    bool read_flag;
    bool write_flag;
    int data;
    int address;
    
public:

    Core();
    ~Core();
    void run();


};

#endif