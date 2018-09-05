#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
using namespace std;

class Clock{

private:
    int cycle_counter;
    
public:

    Clock();
    ~Clock();

    void loop(bool & clk);
    
};


#endif