#include "CPU.h"


/**
 * @brief Construct a new CPU::CPU object
 * 
 * @param id CPU identifier
 */
CPU::CPU(int id){
    this->id = id;
    this->process_timer = 0;
    this->cpu_clk = false;
    this->cpu_state = FREE;
}

/**
 * @brief Destroy the CPU::CPU object
 * 
 */
CPU::~CPU(){}

/**
 * @brief This function shows current processor state and generate random behavior when CPU is free
 * 
 * @param clk Global Clock
 */
void CPU::loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & ready){
    //Check actual state
    switch(this->cpu_state){
        case PROCESSING:
            process();
        break;
        case READING:
            read(data,address,read_flag,ready);
        break;
        case WRITING:
            write(data,address,write_flag,ready);
        break;
        case FREE:
            request();
        break;
    }
}

/**
 * @brief 
 * 
 * @return int 
 */
int CPU::getId(){
    return this->id;
}

/**
 * @brief Print CPU current state
 * 
 */
void CPU::getState(){
     switch(this->cpu_state){
        case PROCESSING:
            cout << "Processing" << endl;
        break;
        case READING:
            cout << "Reading" << endl;
        break;
        case WRITING:
            cout << "Writing" << endl;;
        break;
        case FREE:
            cout << "Free" << endl;
        break;
    }
}

/**
 * @brief 
 * 
 */
void CPU::process(){

    cout << "Processing" << endl;

    //Decrease process timer each cycle
    this->process_timer--;

    //Check if processing time finish
    if(this->process_timer == 0){
        this->cpu_state = FREE;
    }
    
}

/**
 * @brief Write random data in random cache block
 * 
 * @param data core data wire
 * @param address core address wire
 * @param write_flag core write flag
 * @param ready core ready flag
 */
void CPU::write(int & data, int & address, bool & write_flag, bool & ready){
    //Check if write process finish
    if(ready){
        this->cpu_state = FREE;
    }
    //If it is first time setting flag, write the data in random address
    else if(!write_flag){
        data = rand() % 256; //Random 1byte data
        address = rand() % 15; //0-15 memory random block
        write_flag = true;
    }

}

/**
 * @brief Read data from random block address
 * 
 * @param data core data wire
 * @param address core address wire
 * @param read_flag core read flag
 * @param ready core ready flag
 */
void CPU::read(int & data, int & address, bool & read_flag, bool & ready){
    //Check if read process finish
    if(ready){
        cout << "Retrived data: " << data << "from block:" << address << endl;
        this->cpu_state = FREE;
    }
    //If it is first time setting flag, read the data from random address
    else if(!read_flag){
        address = rand() % 15; //0-15 memory random block
        read_flag = true;
    }
}

/**
 * @brief generate random state for CPU
 * 
 */
void CPU::request(){
    this->cpu_state = static_cast<CPU::State>(rand() % 3);
}
