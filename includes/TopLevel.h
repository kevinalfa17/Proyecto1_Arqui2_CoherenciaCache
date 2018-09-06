/**
 * @brief TopLevel with every component and conection
 * 
 * @file TopLevel.h
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

#include "Bus.h"
#include "BusMessage.h"
#include "Cache.h"
#include "Clock.h"
#include "Core.h"
#include "Memory.h"

using namespace std;

#define NUM_CORES 4

class TopLevel{

private:
    

    //Thread functions
    static void * run_clk(void *ptr);
    static void * run_cpu(void *ptr);
    static void * run_control(void *ptr);
    static void * run_cache(void *ptr);
    static void * run_bus(void *ptr);
    static void * run_memory(void *ptr);

public:
    TopLevel();
    ~TopLevel();
    void init();
    
};

#endif