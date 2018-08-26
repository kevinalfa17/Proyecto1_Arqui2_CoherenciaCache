#include "Core.h"


/**
 * @brief Construct a new Core:: Core object
 * 
 * @param id 
 */
Core::Core(int id){
    //Initialize wires and flags
    this->id = id;
    this->address = -1;
    this->data = -1;
    this->ready = false;
    this->read_flag = false;
    this->write_flag = false;
    this->running = false;
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
 * @brief Start core with all components inside (CPU, Cache, Control)
 * 
 * @param clk Global clk
 */
void Core::update(bool clk){

    cpu->loop(clk,this->data, this->address, this->write_flag, this->read_flag, this->ready);

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