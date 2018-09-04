#include "Memory.h"


/**
 * @brief Construct a new Memory:: Memory object
 * 
 */
Memory::Memory(){
    //Initialize control variables
    this->running=false;
    this->memory_clk=false;
    //Initialize memory in zeros
    for(int i = 0; i < 16; i++){
        blocks[i] = i;
    }
}

/**
 * @brief Destroy the Memory:: Memory object
 * 
 */
Memory::~Memory(){}

/**
 * @brief 
 * 
 * @param clk 
 * @param bussy 
 * @param address 
 * @param actualMessage 
 */
void Memory::loop(bool clk, bool & bussy, BusMessage * actualMessage, vector<bool> *snoop_flag, vector<BusMessage*> * queue){
        
    //Posedge
    if (this->memory_clk == false && clk == true)
    {
        this->memory_clk = true;
        cout << "Memory" << endl;
        
        if(bussy == true){
            switch (actualMessage->getType()){
                //READ
                case 0:
                    cout <<"########## MEMORY: Search for data in address:"<<actualMessage->getAddress()<<"##########"<<endl;
                    actualMessage->setType(2);
                    actualMessage->setData(this->blocks[actualMessage->getAddress()]);
                    cout << "########## Done Read ##########" <<  endl;

                    //Set flags for every core
                    this->broadcast(snoop_flag);
                    //Free  bus
                    bussy = false;
                    queue->erase(queue->begin());

                    break;
                //WRITE
                case 1:
                    cout <<"########## MEMORY: Write in address: "<<actualMessage->getAddress()<<" data:"<<actualMessage->getData()<<" ##########"<<endl;
                    this->blocks[actualMessage->getAddress()] = actualMessage->getData();
                    actualMessage->setType(3);
                    cout << "########## Done Write ##########" <<  endl;

                    //Set flags for every core
                    this->broadcast(snoop_flag);
                    bussy = false;
                    //Free bus
                    queue->erase(queue->begin());
                    break;
            }
                

        }
    }
    //Nededge
    else if (this->memory_clk == true && clk == false){
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