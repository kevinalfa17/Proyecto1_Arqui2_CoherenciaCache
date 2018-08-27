#include "Core.h"


/**
 * @brief Construct a new Core:: Core object
 * 
 * @param id 
 */
Core::Core(int id){
    //Initialize flags
    this->id = id;
    this->control_clk = false;
    this->running = false;

    //Initialize wires
    this->data_cache = -1;
    this->address_cache = -1;
    this->read_flag_cache = false;
    this->write_flag_cache = false;

    this->data_cpu = -1;
    this->address_cpu = -1;
    this->read_flag_cpu = false;
    this->write_flag_cpu = false;

    this->snoop_flag_bus = false;
    this->snoop_flag_cache = false;
    
    this->ready = false;
    

    //Initialize components
    this->cpu = new CPU(id);
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
void Core::cpu_loop(bool clk){

    cpu->loop(clk,this->data_cpu, this->address_cpu, this->write_flag_cpu, this->read_flag_cpu, this->ready);

}

/**
 * @brief Start cache
 * 
 * @param clk Global clk
 */
void Core::cache_loop(bool clk){



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