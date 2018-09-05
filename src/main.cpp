#include "Bus.h"
#include "BusMessage.h"
#include "Cache.h"
#include "Clock.h"
#include "Core.h"
#include "Memory.h"

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

using namespace std;

#define NUM_CORES 4

pthread_mutex_t mutex; 

//Enable signals
bool cpuEnable[4];
bool controlEnable[4];
bool cacheEnable[4];
bool busEnable[4];
bool memoryEnable;

//Clock variables
bool global_clk;

//Conection components
vector<BusMessage*> * queue;
BusMessage* actualMessage;
vector<bool> * snoop_flag;
bool bussy;

//Components
Core * core[NUM_CORES];
Clock * clk;
Bus * bus;
Memory * memory;


void * run_clk(void *ptr){

    Clock * clk = ((Clock *)ptr);  

    while(true){

        //Control logic
        int next = -1;
        cin>>next;
        if(next == 0){
            //Clock cycle
            clk->loop(global_clk);
            cpuEnable[0] = true;
            cpuEnable[1] = true;
            cpuEnable[2] = true;
            cpuEnable[3] = true;

            next = -1;
        }
        else if(next == 1){
            int numcore = 0;
            cout<<"Escriba el numero de core del que desea ver la cache"<<endl;
            cin>>numcore;
            core[numcore]->printCache();

            next = -1;
        }
        else if(next == 2){
            next = -1;
            memory->printData();
        }
    }  
}

void * run_cpu(void *ptr){

    Core * core = ((Core *)ptr);    
    int id = core->getId();

    //If core is running, update clk
    while(core->isRunning()){
         pthread_mutex_lock(&mutex); //Lock access

         //Check enable signal
        if(cpuEnable[id] == true){
            cpuEnable[id] = false;
            core->cpu_loop(global_clk, controlEnable[id]);
        }
        pthread_mutex_unlock(&mutex); //Unlock access

    }
}

void * run_control(void *ptr){

    Core * core = ((Core *)ptr);
    int id = core->getId();

    //If core is running, update clk
    while(core->isRunning()){
        
        pthread_mutex_lock(&mutex); //Lock access

        //Check enable signal
        if(controlEnable[id] == true){
            controlEnable[id] = false;
            core->control_loop(global_clk, queue, snoop_flag, actualMessage, busEnable[id], cacheEnable[id]);
        }
        pthread_mutex_unlock(&mutex); //Unlock access

    }
}

void * run_cache(void *ptr){

    Core * core = ((Core *)ptr);
    int id = core->getId();

    //If core is running, update clk
    while(core->isRunning()){
        
        pthread_mutex_lock(&mutex); //Lock access

        //Check enable signal
        if(cacheEnable[id] == true){
            cacheEnable[id] = false;
            core->cache_loop(global_clk);
        }
        pthread_mutex_unlock(&mutex); //Unlock access

    }
}

void * run_bus(void *ptr){

    Bus * bus = ((Bus *)ptr);

    //If bus is running, update clk
    while(bus->isRunning()){
        pthread_mutex_lock(&mutex); //Lock access

        //Check enable signals
        if(busEnable[0] == true && busEnable[1] == true && busEnable[2] == true && busEnable[3] == true){
            busEnable[0] = false;
            busEnable[1] = false;
            busEnable[2] = false;
            busEnable[3] = false;
            bus->loop(global_clk, queue, snoop_flag, actualMessage, bussy, memoryEnable);           
        }
        pthread_mutex_unlock(&mutex); //Unlock access
    }
}


void * run_memory(void *ptr){

    Memory * memory = ((Memory *)ptr);

    //If memory is running, update clk
    while(memory->isRunning()){
        pthread_mutex_lock(&mutex); //Lock access

        //Check enable signal
        if(memoryEnable == true){
            memoryEnable = false;
            memory->loop(global_clk, bussy, actualMessage, snoop_flag, queue);
        }
        pthread_mutex_unlock(&mutex); //Unlock access
    }

}

int main(){

    //Conection variables
    queue = new vector<BusMessage*>();
    actualMessage = new BusMessage(-2, -2, -2, -2);
    snoop_flag = new vector<bool>();
    global_clk = false;
    bussy = false;

    //Flags initialization
    for(int i = 0; i < NUM_CORES; i++){
        snoop_flag->push_back(0);

        cpuEnable[i] = false;
        controlEnable[i] = false;
        cacheEnable[i] = false;
        busEnable[i] = false; 
    }
    memoryEnable = false;

    

    //Cache * cache = new Cache(1);
    //cache->printMemory();

    //Cores initialization
    for(int i = 0; i < NUM_CORES; i++){
        core[i] = new Core(i);
        core[i]->run();
    }

    //Create clock
    clk = new Clock();

    //Create bus
    bus = new Bus();

    //Create memory
    memory = new Memory();

    //Start bus
    bus->run();

    //Start memory
    memory->run();

    //Create the threads
    pthread_t thread_cpu[NUM_CORES], thread_control[NUM_CORES], thread_cache[NUM_CORES], thread_bus, thread_memory, thread_clk;
    
    pthread_attr_t attr;
    void* ret;

    //Set mutex, condition variables and attributes
    pthread_mutex_init(&mutex, NULL); 
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Start the threads
    for(int i = 0; i < NUM_CORES; i++){
        pthread_create(&thread_cpu[i], &attr, &run_cpu, core[i]);
        pthread_create(&thread_control[i], &attr, &run_control, core[i]);
        pthread_create(&thread_cache[i], &attr, &run_cache, core[i]);
    }

    pthread_create(&thread_clk, &attr, &run_clk, clk);
    pthread_create(&thread_bus, &attr, &run_bus, bus);
    pthread_create(&thread_memory, &attr, &run_memory, memory);
    

    //Wait for threads to finish
    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(thread_cpu[i], &ret);
        pthread_join(thread_control[i], &ret);
        pthread_join(thread_cache[i], &ret);
    }

    pthread_join(thread_clk, &ret);
    pthread_join(thread_bus, &ret);
    pthread_join(thread_memory, &ret);
    
    
    //Destroy mutex and attributes
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
