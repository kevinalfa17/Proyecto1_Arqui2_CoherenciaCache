/**
 * @brief Bus model
 * 
 * @file Bus.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#include "Bus.h"

/**
 * @brief Construct a new Bus:: Bus object
 * 
 */
Bus::Bus()
{
    this->id = -1;
    this->bus_clk = false;
    this->running = false;
}

/**
 * @brief Destroy the Bus:: Bus object
 * 
 */
Bus::~Bus(){}

/**
 * @brief Update bus every clock
 * 
 * @param clk 
 * @param queue 
 * @param snoop_flag 
 * @param actualMessage 
 * @param bussy 
 * @param memoryEnable 
 */
void Bus::loop(bool clk, vector<BusMessage *> *queue, vector<bool> *snoop_flag, BusMessage * actualMessage, bool & bussy, bool & memoryEnable)
{

    //Posedge
    if (this->bus_clk == false && clk == true)
    {
        this->bus_clk = true;
        //cout << "Bus" << endl;

        cout <<"\n-------------COLA EN EL BUS-------------"<<endl;
        for(int i = 0; i<queue->size(); i++){  

            if(queue->at(i)->getType()== 0){
                cout<<"CPU"<< queue->at(i)->getId()<< " type: Read  Address: "<<queue->at(i)->getAddress()<<endl;
            }
            else if(queue->at(i)->getType() == 1){
                cout<<"CPU"<< queue->at(i)->getId()<< " type: Write  Address: "<<queue->at(i)->getAddress()<<" Data: "<<queue->at(i)->getData()<<endl;
            }
            else{
                cout<<"CPU"<< queue->at(i)->getId()<< " type: Ready  Address: "<<queue->at(i)->getAddress()<<endl;

            }

            
        }

        if (queue->size() > 0 && bussy == false){ //There are request to process

            *actualMessage = *queue->front();
            bussy = true; //Do not process more message in queue for now
            this->broadcast(snoop_flag); //Broadcast message to every core
        }
         cout <<"----------------------------------------\n"<<endl;

        if(actualMessage->getType()== 0){
            cout <<"-----------ActualMessage CPU"<< actualMessage->getId()<<" Type: Read "<<" Address "<<actualMessage->getAddress()<<"----------"<<endl;

        }
        else if(actualMessage->getType() == 1){
            cout <<"-----------ActualMessage CPU"<< actualMessage->getId()<<" Type: Write "<<" Address "<<actualMessage->getAddress()<<" Data: "<<actualMessage->getData()<<"----------"<<endl;

        }
        else{
            cout <<"-----------ActualMessage CPU"<< actualMessage->getId()<<" Type: Ready "<<" Address "<<actualMessage->getAddress()<<"----------"<<endl;

        }

        memoryEnable = true;
       
    }
    //Nededge
    else if (this->bus_clk == true && clk == false){
        //cout << "Bus Nededge" << endl;

        this->bus_clk = false;
        memoryEnable = true;
      
    }
}

/**
 * @brief Tell every core about new message
 * 
 * @param snoop_flag 
 */
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
