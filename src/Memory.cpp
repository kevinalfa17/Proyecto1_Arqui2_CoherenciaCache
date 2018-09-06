/**
 * @brief Memory model
 * 
 * @file Memory.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#include "Memory.h"


/**
 * @brief Construct a new Memory:: Memory object
 * 
 */
Memory::Memory(){
    //Initialize control variables
    this->running=false;
    this->memory_clk=false;
    this->counter = 0;
    //Initialize memory in zeros
    for(int i = 0; i < 16; i++){
        blocks[i] = 0;
    }
}

/**
 * @brief Destroy the Memory:: Memory object
 * 
 */
Memory::~Memory(){}

/**
 * @brief Uṕdate memory every clock
 * 
 * @param clk 
 * @param bussy 
 * @param actualMessage 
 * @param snoop_flag 
 * @param queue 
 */
void Memory::loop(bool clk, bool & bussy, BusMessage * actualMessage, vector<bool> *snoop_flag, vector<BusMessage*> * queue){
        
    //Posedge
    if (this->memory_clk == false && clk == true)
    {
        this->memory_clk = true;
        //cout << "Memory" << endl;
        
        if(bussy == true){
            switch (actualMessage->getType()){
                //READ
                case 0:

                    if(this->counter == 1){
                        actualMessage->setData(this->blocks[actualMessage->getAddress()]);
                        //Set flags for every core
                        actualMessage->setType(2);
                        this->broadcast(snoop_flag);

                        //Free  bus
                        bussy = false;
                        queue->erase(queue->begin());
                        this->counter = 0;
                        cout << "########## MEMORY: Done read ##########" <<  endl;
                    }
                    else if(this->counter == 0){
                        cout <<"\n########## MEMORY: Search for data in address:"<<actualMessage->getAddress()<<"##########"<<endl;
                        this->counter++;
                    }
                    
                    break;
                //WRITE
                case 1:

                    if(this->counter == 1){
                        this->blocks[actualMessage->getAddress()] = actualMessage->getData();                        
                        //Set flags for every core
                        actualMessage->setType(3);
                        this->broadcast(snoop_flag);
                        
                        
                        //Free bus
                        bussy = false;
                        queue->erase(queue->begin());
                        this->counter = 0;

                        cout << "########## MEMORY: Done write ##########" <<  endl;
                    }
                    else if(this->counter == 0){
                        cout <<"########## MEMORY: Write in address: "<<actualMessage->getAddress()<<" data:"<<actualMessage->getData()<<" ##########"<<endl;
                        this->counter++;
                    }

                    break;
            }
                

        }
    }
    //Nededge
    else if (this->memory_clk == true && clk == false){
                //cout << "Memory Nededge" << endl;

        this->memory_clk = false;
    }
}

/**
 * @brief 
 * 
 */
void Memory::printData(){
    cout<<"##########Memory content##########"<<endl;
    for(int i = 0; i<16; i++){
        cout<<"## "<<i<<" ## "<<this->blocks[i]<<" #############"<<endl;
    }
    cout<<"##################################"<<endl;
}

/**
 * @brief 
 * 
 * @param snoop_flag 
 */
void Memory::broadcast(vector<bool> *snoop_flag)
{
    //Set snoop flag true for every core
    for (int i = 0; i < snoop_flag->size(); i++)
    {
        snoop_flag->at(i) = true;
    }
}

/**
 * @brief Start memory
 * 
 */
void Memory::run(){
    this->running = true;
}

/**
 * @brief Stop memory activity
 * 
 */
void Memory::stop(){
    this->running = false;
}

/**
 * @brief Check if memory is running
 * 
 * @return true Is running
 * @return false Is stopped
 */
bool Memory::isRunning(){
    return this->running;
}