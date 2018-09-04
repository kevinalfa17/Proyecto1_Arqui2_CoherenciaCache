#include "Bus.h"

Bus::Bus()
{
    this->id = -1;
    this->bussy = false;
    this->bus_clk = false;
    this->handler = -1;
    this->running = false;
}

Bus::~Bus(){}

void Bus::loop(bool clk, vector<BusMessage *> *queue, vector<bool> *snoop_flag, BusMessage * actualMessage)
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

        if (queue->size() > 0 && this->bussy == false){ //There are request to process

            *actualMessage = *queue->front();
            this->bussy = true; //Do not process more message in queue for now
            this->broadcast(snoop_flag); //Broadcast message to every core
        }
         cout <<"-------------------------------"<<endl;

        cout <<"-----------ActualMessage ID "<< actualMessage->getId()<<" Address "<< actualMessage->getAddress()<<" Type "<< actualMessage->getType()<<"----------"<<endl;
       

        //ESTO LO HARIA LA MEMORIA
        if(this->bussy == true){
            switch (actualMessage->getType()){
                //READ
                case 0:
                    //SEARCH AND FIND IN MEMORY
                    cout <<"--SEARCH AND FIND--"<<endl;
                    actualMessage->setType(2);
                    actualMessage->setData(69);
                    cout << "--READY FROM BUS-- TYPE: " <<actualMessage->getType()<<  endl;
                    this->broadcast(snoop_flag);
                    this->bussy = false;
                    queue->erase(queue->begin());
                    break;
                //WRITE
                case 1:
                    //WRITE IN MEMORY
                    cout <<"--WRITE MEMORY--"<<endl;
                    actualMessage->setType(3);
                    cout << "--READY FROM BUS-- TYPE: " <<actualMessage->getType()<<  endl;
                    this->broadcast(snoop_flag);
                    this->bussy = false;
                    queue->erase(queue->begin());
                    break;
                }
                

        }
        //FIN ESTO LO HARIA LA MEMORIA

        /*if(this->bussy == true){
            
            //Read/Write message is ready
            if(actualMessage->getType() == 2 || actualMessage->getType() == 3){
                cout << "--READY FROM BUS-- TYPE: " <<actualMessage->getType()<<  endl;
                this->broadcast(snoop_flag);
                this->bussy = false;
                queue->erase(queue->begin());
            }
        }*/
    }
    //Nededge
    else if (this->bus_clk == true && clk == false){
        this->bus_clk = false;

        //cout << "Bus nededge" << endl;

        //ESTO LO HARIA LA MEMORIA
        /*if(this->bussy){
            switch (actualMessage->getType()){
                //READ
                case 0:
                    //SEARCH AND FIND IN MEMORY
                    cout <<"--SEARCH AND FIND--"<<endl;
                    actualMessage->setType(2);
                    actualMessage->setData(69);
                    break;
                //WRITE
                case 1:
                    //WRITE IN MEMORY
                    cout <<"--WRITE MEMORY--"<<endl;
                    actualMessage->setType(3);
                    break;
            }
        }*/
        //FIN ESTO LO HARIA LA MEMORIA
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
