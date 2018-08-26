#include "Core.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_mutex_t mutex; 

bool global_clk = false;


void * run_clk(void *ptr){
    while(true){
        pthread_mutex_lock(&mutex); //Lock access
        global_clk = !global_clk;
        cout<<"global "<<global_clk<<endl;

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

    Core core1 = Core(1);

    //Create the threads
    pthread_t thread_core, thread_clk;
    pthread_attr_t attr;
    void* ret;

    //Set mutex and attributes
    pthread_mutex_init(&mutex, NULL); 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Start the threads
    pthread_create(&thread_core, &attr, &run_core, &core1);
    pthread_create(&thread_clk, &attr, &run_clk, NULL );

    //Wait for threads to finish
    pthread_join(thread_core, &ret);
    pthread_join(thread_clk, &ret);
    
    //Destroy mutex and attributes
    pthread_attr_destroy(&attr); 
    pthread_mutex_destroy(&mutex);


    return 0;
}
