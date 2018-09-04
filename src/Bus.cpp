#include "Bus.h"

Bus::Bus()
{
    this->id = -1;
    this->bus_clk = false;
    this->running = false;
}

Bus::~Bus(){}

void Bus::loop(bool clk, vector<BusMessage *> *queue, vector<bool> *snoop_flag, BusMessage * actualMessage,bool & bussy)
{

    //Posedge
    if (this->bus_clk == false && clk == true)
    {
        this->bus_clk = true;
        cout << "Bus" << endl;

        cout <<"----------COLA BUS----------"<<endl;
        for(int i = 0; i<queue->size(); i++){
            string type;

            if(queue->at(i)->getType()== 0){
                type = "Read";
            }
            else if(queue->at(i)->getType() == 1){
                type = "Write";
            }
            else{
                type = "Ready";
            }

            cout<<"CPU"<< queue->at(i)->getId()<< " type: "<<type<<" Address: "<<queue->at(i)->getAddress()<<endl;
        }

        if (queue->size() > 0 && bussy == false){ //There are request to process

            *actualMessage = *queue->front();
            bussy = true; //Do not process more message in queue for now
            this->broadcast(snoop_flag); //Broadcast message to every core
        }
         cout <<"-------------------------------"<<endl;

        string type;
        if(actualMessage->getType()== 0){
            type = "Read";
        }
        else if(actualMessage->getType() == 1){
            type = "Write";
        }
        else{
            type = "Ready";
        }

        cout <<"-----------ActualMessage CPU"<< actualMessage->getId()<<" Address "<< actualMessage->getAddress()<<" Type "<<type<<"----------"<<endl;
       

       
    }
    //Nededge
    else if (this->bus_clk == true && clk == false){
        this->bus_clk = false;
      
    }
}

void Bus::broadcast(vector<bool> *snoop_flag)
{
    //Set snoop flag true for every core
    for (int i = 0; i < snoop_flag->size(); i++)
    {
        snoop_flag->at(i) = true;
    }
}

/**
 * @brief Start bus
 * 
 */
void Bus::run(){
    this->running = true;
}

/**
 * @brief Stop bus activity
 * 
 */
void Bus::stop(){
    this->running = false;
}

/**
 * @brief Check if bus is running
 * 
 * @return true Is running
 * @return false Is stopped
 */
bool Bus::isRunning(){
    return this->running;
}
