#include "Core.h"
#include "Cache.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_mutex_t mutex; 

bool global_clk = false;
int cycle_counter = 0;


void * run_clk(void *ptr){
    while(true){
        pthread_mutex_lock(&mutex); //Lock access
        global_clk = !global_clk;

        //Print cycle number in the posedge
        if(global_clk){
            cout<<"Cycle: "<<cycle_counter<<endl;
            cycle_counter++;
        }

        pthread_mutex_unlock(&mutex); //Unlock access
        sleep(5);
    }  
}

void * run_core(void *ptr){

    Core * core = ((Core *)ptr);

    //Start core
    core->run();

    //If core is running, update clk
    while(core->isRunning()){
        pthread_mutex_lock(&mutex); //Lock access
        core->update(global_clk);
        pthread_mutex_unlock(&mutex); //Unlock access
    }
}

int main(){

    Cache * cache = new Cache(1);
    cache->printMemory();

    /*
    Core core1 = Core(1);
    Core core2 = Core(2);

    //Create the threads
    pthread_t thread_core1, thread_core2, thread_clk;
    pthread_attr_t attr;
    void* ret;

    //Set mutex and attributes
    pthread_mutex_init(&mutex, NULL); 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Start the threads
    pthread_create(&thread_core1, &attr, &run_core, &core1);
    pthread_create(&thread_core2, &attr, &run_core, &core2);
    pthread_create(&thread_clk, &attr, &run_clk, NULL );

    //Wait for threads to finish
    pthread_join(thread_core1, &ret);
    pthread_join(thread_core2, &ret);
    pthread_join(thread_clk, &ret);
    
    //Destroy mutex and attributes
    pthread_attr_destroy(&attr); 
    pthread_mutex_destroy(&mutex);
    */


    return 0;
}
