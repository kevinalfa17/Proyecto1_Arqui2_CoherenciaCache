#include "CPU.h"

#define MEMORY_BLOCKS 5


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
    srand(time(NULL)); //To get more random numbers
}

/**
 * @brief Destroy the CPU::CPU object
 * 
 */
CPU::~CPU(){}

/**
 * @brief Update cpu every clock
 * 
 * @param clk Global clock
 * @param data data from/to cache control unit
 * @param address to cache control unit
 * @param write_flag true if cpu needs to write
 * @param read_flag true if cpu needs to read
 * @param ready read/write ready
 */
void CPU::loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & ready){

    //Posedge
    if(this->cpu_clk == false && clk == true){
        this->cpu_clk = true;
        //if(this->id == 1)
        //cout<<"CPU: "<< this->id <<endl;

        //Start new process if CPU is free
        if(this->cpu_state == FREE){
            request();
        }
        
        //Print actual state
        //this->getState();

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
        }

    }
    //Nededge
     else if(this->cpu_clk == true && clk == false){
        this->cpu_clk = false;
        //cout << "CPU nededge" << endl;

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
            cout <<"CPU: "<<this->id<<" Processing" << endl;
        break;
        case READING:
            cout <<"CPU: "<<this->id<< " Reading" << endl;
        break;
        case WRITING:
            cout <<"CPU: "<<this->id << " Writing" << endl;;
        break;
        case FREE:
            cout <<"CPU: "<<this->id << "Free" << endl;
        break;
    }
}

/**
 * @brief Process data simulation
 * 
 */
void CPU::process(){

    //cout << "Processing" << endl;

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
    if(ready && write_flag){
        cout <<"CPU: "<<this->id<< " ******** Written data: " << data << " to block: " << address <<" ********"<< endl;
        
        ready = false;
        write_flag = false;
        this->cpu_state = FREE;
    }
    //If it is first time setting flag, write the data in random address
    else if(!write_flag){
        data = rand() % 256; //Random 1byte data
        address = rand() % MEMORY_BLOCKS; //0-15 memory random block
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
    if(ready && read_flag){
        cout <<"CPU: "<<this->id<< " ******** Retrived data: " << data << " from block: " << address <<" **********" <<endl;
        
        ready = false;
        read_flag = false;
        this->cpu_state = FREE;
    }
    //If it is first time setting flag, read the data from random address
    else if(!read_flag){
        address = rand() % MEMORY_BLOCKS; //0-15 memory random block
        read_flag = true;
    }
}

/**
 * @brief generate random state for CPU
 * 
 */
void CPU::request(){
    this->cpu_state = static_cast<CPU::State>(rand() % 3);
    //this->cpu_state = WRITING;

    //Reset processing timer variable
    if(this->cpu_state == PROCESSING){
        this->cpu_state = WRITING;
        //this->process_timer = 2; //Make it random
    }
}
