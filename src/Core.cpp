#include "Core.h"


/**
 * @brief Construct a new Core:: Core object
 * 
 * @param id 
 */
Core::Core(int id){
    //Initialize flags
    this->id = id;
    this->running = false;

    //Initialize wires
    this->data = -1;
    this->address = -1;
    this->snoop_address_cache = -1;
    this->snoop_data_cache = -1;

    this->ready = false;

    this->read_flag_cpu = false;
    this->write_flag_cpu = false;

    this->read_flag_cache = false;
    this->write_flag_cache = false;
    this->snoop_flag_cache = false;  

    //Initialize components
    this->cpu = new CPU(id);
    this->control = new Control(id);
}

/**
 * @brief Destroy the Core:: Core object
 * 
 */
Core::~Core(){
    delete cpu;
}

/**
 * @brief Start cpu
 * 
 * @param clk Global clk
 */
void Core::cpu_loop(bool clk, vector<BusMessage*> * CPU_queue, bool cycle_counter, bool debug){

    cpu->loop(clk,this->data, this->address, this->write_flag_cpu, this->read_flag_cpu, this->ready,CPU_queue,cycle_counter,debug);

}

/**
 * @brief Start cache
 * 
 * @param clk Global clk
 */
void Core::cache_loop(bool clk){



}

/**
 * @brief Start control unit
 * 
 * @param clk Global clk
 */
void Core::control_loop(bool clk, vector<BusMessage*> * queue, vector<bool> * snoop_flag, BusMessage * actualMessage){

    control->loop(clk,this->data, this->address, this->write_flag_cpu, this->read_flag_cpu, this->write_flag_cache, this->read_flag_cache, this->ready, snoop_flag, queue, actualMessage, this->snoop_flag_cache, this->snoop_data_cache, this->snoop_address_cache);

}

/**
 * @brief Start core
 * 
 */
void Core::run(){
    this->running = true;
}

/**
 * @brief Stop all core activities
 * 
 */
void Core::stop(){
    this->running = false;
}

/**
 * @brief Check if core is running
 * 
 * @return true Is running
 * @return false Is stopped
 */
bool Core::isRunning(){
    return this->running;
}

/**
 * @brief get core number
 * 
 * @return int 
 */
int Core::getId(){
    return this->id;
}