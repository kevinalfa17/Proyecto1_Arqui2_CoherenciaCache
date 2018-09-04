#include "Core.h"
#include "Cache.h"
#include "Bus.h"
#include "BusMessage.h"
#include "Memory.h"

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

using namespace std;

pthread_mutex_t mutex[4]; 
pthread_mutex_t bus_mutex; 
pthread_cond_t cv1[4];
pthread_cond_t cv2;

bool core_pending[4];
bool global_clk = false;
int cycle_counter = 0;

vector<BusMessage*> * queue;
BusMessage* actualMessage;
vector<bool> * snoop_flag;
bool bussy;


void * run_clk(void *ptr){

    while(true){
        int next;
        //cout <<"Enter to continue 0"<<endl;
        cin>>next;

        if(next == 0){
            global_clk = !global_clk;
            //Print cycle number in the posedge
            if(global_clk){
                cout<<"---------------------------Posedge: Cycle: "<<cycle_counter<<"----------------------------------"<<endl;
                sleep(1);
                cycle_counter++;
            }
            else{
                cout<<"---------------------------Nededge: Cycle: "<<cycle_counter<<"----------------------------------"<<endl;
                sleep(1);
                }
            next = 1;
        }
    }  
}

void * run_cpu(void *ptr){

    Core * core = ((Core *)ptr);    
    int id = core->getId();

    //If core is running, update clk
    while(core->isRunning()){
        pthread_mutex_lock(&mutex[id]); //Lock access
        core->cpu_loop(global_clk);
        pthread_cond_signal(&cv1[id]);//CPU-Control synchronization
        pthread_mutex_unlock(&mutex[id]); //Unlock access
    }
}

void * run_control(void *ptr){

    Core * core = ((Core *)ptr);
    int id = core->getId();

    //If core is running, update clk
    while(core->isRunning()){
        
        pthread_mutex_lock(&bus_mutex); //Lock access
        pthread_mutex_lock(&mutex[id]); //Lock access
        pthread_cond_wait(&cv1[id],&mutex[id]);//CPU-Control synchronization
        core->control_loop(global_clk, queue, snoop_flag, actualMessage);
        //Flag for bus access
        if(core_pending[id] == false){
            core_pending[id] = true;
        }

        pthread_mutex_unlock(&mutex[id]); //Unlock access
        pthread_mutex_unlock(&bus_mutex); //Unlock access

    }
}

void * run_bus(void *ptr){

    Bus * bus = ((Bus *)ptr);

    //If bus is running, update clk
    while(bus->isRunning()){
        pthread_mutex_lock(&bus_mutex); //Lock access
        if(core_pending[0] == true && core_pending[1] == true && core_pending[2] == true && core_pending[3] == true){
            core_pending[0] = false;
            core_pending[1] = false;
            core_pending[2] = false;
            core_pending[3] = false;
            bus->loop(global_clk, queue, snoop_flag, actualMessage, bussy);
            pthread_cond_signal(&cv2);//Bus-Memory synchronization
           
        }
        pthread_mutex_unlock(&bus_mutex); //Unlock access
    }
}


void * run_memory(void *ptr){

    Memory * memory = ((Memory *)ptr);

    //If memory is running, update clk
    while(memory->isRunning()){
        pthread_mutex_lock(&bus_mutex); //Lock access
        pthread_cond_wait(&cv2,&bus_mutex);//Bus-Memory synchronization
        memory->loop(global_clk, bussy, actualMessage, snoop_flag, queue);
        pthread_mutex_unlock(&bus_mutex); //Unlock access
    }

}

int main(){

    //constructor top level
    queue = new vector<BusMessage*>();
    actualMessage = new BusMessage(-2, -2, -2, -2);
    snoop_flag = new vector<bool>();
    bussy = false;

    //Flags initialization
    for(int i = 0; i < 4; i++){
        snoop_flag->push_back(0);
        core_pending[i] = false;
    }
    //constructor top level

    //Cache * cache = new Cache(1);
    //cache->printMemory();

    //Create cores
    Core * core1 = new Core(0);
    Core * core2 = new Core(1);
    Core * core3 = new Core(2);
    Core * core4 = new Core(3);

    //Create bus
    Bus * bus = new Bus();

    //Create memory
    Memory * memory = new Memory();

    //Start cores
    core1->run();
    core2->run();
    core3->run();
    core4->run();

    //Start bus
    bus->run();

    //Start memory
    memory->run();

    //Create the threads
    pthread_t thread_cpu1,thread_cpu2,thread_cpu3,thread_cpu4,thread_control1,thread_control2,thread_control3,thread_control4, thread_bus, thread_memory, thread_clk;
    pthread_attr_t attr;
    void* ret;

    //Set mutex, condition variables and attributes
    for(int i = 0; i > 4; i++){
        pthread_mutex_init(&mutex[i], NULL); 
        pthread_cond_init(&cv1[i], 0);
    }
    pthread_cond_init(&cv2, 0);
    pthread_mutex_init(&bus_mutex, NULL); 
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Start the threads
    pthread_create(&thread_clk, &attr, &run_clk, NULL );

    pthread_create(&thread_cpu1, &attr, &run_cpu, core1);
    pthread_create(&thread_control1, &attr, &run_control, core1);

    pthread_create(&thread_cpu2, &attr, &run_cpu, core2);
    pthread_create(&thread_control2, &attr, &run_control, core2);

    pthread_create(&thread_cpu3, &attr, &run_cpu, core3);
    pthread_create(&thread_control3, &attr, &run_control, core3);

    pthread_create(&thread_cpu4, &attr, &run_cpu, core4);
    pthread_create(&thread_control4, &attr, &run_control, core4);

    pthread_create(&thread_bus, &attr, &run_bus, bus);
    pthread_create(&thread_memory, &attr, &run_memory, memory);

    

    //Wait for threads to finish
    pthread_join(thread_cpu1, &ret);
    pthread_join(thread_control1, &ret);
    

    pthread_join(thread_cpu2, &ret);
    pthread_join(thread_control2, &ret);

    pthread_join(thread_cpu3, &ret);
    pthread_join(thread_control3, &ret);

    pthread_join(thread_cpu4, &ret);
    pthread_join(thread_control4, &ret);

    pthread_join(thread_bus, &ret);

    pthread_join(thread_memory, &ret);

    pthread_join(thread_clk, &ret);
    
    //Destroy mutex and attributes
    pthread_attr_destroy(&attr); 
    for(int i = 0; i > 4; i++){
        pthread_mutex_destroy(&mutex[i]);
        pthread_cond_destroy(&cv1[i]); 
    }
    pthread_cond_destroy(&cv2);
    pthread_mutex_destroy(&bus_mutex);
    
    return 0;
}
