#include "Control.h"

/**
 * @brief Construct a new Control:: Control object
 * 
 * @param id Control identifier
 */
Control::Control(int id){
    this->id = id;
    this->control_clk = false;
    this->wait = false;
    this->initStates();
}

/**
 * @brief Destroy the Control:: Control object
 * 
 */
Control::~Control(){
    delete this->states;
}

void Control::initStates(){
    this->states = new vector<State>();
    for(int i = 0; i < 16; i++){
        State state = INVALID;
        this->states->push_back(state);
    }

}

Control::State Control::getState(int block_number){
    return this->states->at(block_number);
}

void Control::setState(int block_number,Control::State state){
    this->states->at(block_number) = state;
}

string Control::getStateString(int block_number){
    switch(this->states->at(block_number)){
        case INVALID:
            return "Invalid";
        break;
        case MODIFIED:
            return "Modified";
        break;
        case SHARED:
            return "Shared";
        break;
    }
}


/**
 * @brief 
 * 
 * @param clk Global clock
 * @param data data from/to cpu
 * @param address address from/to cpu
 * @param write_flag_cpu true if cpu needs to write
 * @param read_flag_cpu true if cpu needs to read
 * @param write_flag_cache true if control unit needs to write in the cache
 * @param read_flag_cache true if cpu needs to write in the cache
 * @param snoop_flag true if snoop is in the bus
 * @param snoop_address address from snoop message
 * @param snoop_type type from snoop message
 * @param snoop_data data from snoop message
 */
void Control::loop(bool clk,int & data, int & address, bool & write_flag_cpu, bool & read_flag_cpu, bool & write_flag_cache, bool & read_flag_cache, bool & ready, vector<bool> * snoop_flag,  vector<BusMessage*> * queue, BusMessage * actualMessage, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache){


    //Posedge
    if(this->control_clk == false && clk == true){
        this->control_clk = true;
        //if(this->id == 1)
        //cout<<"Control unit: "<< this->id <<endl;
        int state;

        //cout<<"wait: "<< this->wait <<endl;

        //Read data in memory block
        if(read_flag_cpu && !this->wait && !ready){


            //Get state of block_number = address
            state = this->states->at(address);

            cout <<"****************Se hará un READ en: "<<this->id<<" A la direccion: "<<address<<"***************"<<endl;


            switch(state){
                case Control::INVALID:
                    cout << "*********************INVALID: Read Miss in cache: " << this->id <<"******************"<<endl;

                    //Put read message in bus queue
                    BusMessage * message;
                    message = new BusMessage(this->id,0,address,data);
                    queue->push_back(message);

                    this->wait = true; //This variable avoid to repeat this process if message was already sent to bus

                    /*      cout <<"COLA"<<endl;
                    for(int i = 0; i<queue->size(); i++){
                        cout<<"i: "<<i<<" id: "<<queue->at(i)->getId()<<" type: "<<queue->at(i)->getType()<<" Address: "<<queue->at(i)->getAddress()<<endl;
                    }*/


                    break;
                case Control::MODIFIED:
                    //HIT
                    cout << "******************MODIFIED: Read Hit in cache: " << this->id <<"******************"<<endl;
                    //Traer dato de cache

                    ready = true;
                    data = 68;

                    //Free controller unit
                    this->wait = false;

                    
                    break;
                case Control::SHARED:
                    //HIT
                    cout << "******************SHARED: Read Hit in cache: " << this->id <<"******************"<<endl;
                    //Traer dato de cache

                    ready = true;
                    data = 68;

                    //Free controller unit
                    this->wait = false;
                    break;
                }
                

            
        }
        //Write data in memory block
        else if(write_flag_cpu && !this->wait && !ready){


            //Get state of block_number = address
            state = this->states->at(address);

            cout <<"****************Se hará un WRITE en: "<<this->id<<" A la direccion: "<<address<<"***************"<<endl;


            switch(state){
                case Control::INVALID:
                    cout << "******************INVALID: Write Miss in cache: " << this->id <<"******************"<< endl;
                    break;
                case Control::MODIFIED:
                    cout << "******************MODIFIED: Write Hit in cache, write through: " << this->id <<"******************"<<  endl;
                    break;
                case Control::SHARED:
                     cout << "******************MODIFIED: Write Hit in cache, write through: " << this->id <<"******************"<<  endl;
                    break;
            }
            
            //Put write message in bus queue
            BusMessage * message;
            message = new BusMessage(this->id,1,address,data);
            queue->push_back(message);
            this->wait = true;

            /*cout <<" Control:  "<<this->id<<" COLA EN WRITE"<<endl;
            for(int i = 0; i<queue->size(); i++){
                cout<<"i: "<<i<<" id: "<<queue->at(i)->getId()<<" type: "<<queue->at(i)->getType()<<" Address: "<<queue->at(i)->getAddress()<<endl;
            }*/

                    
        }

    }
    //Nededge
    else if(this->control_clk == true && clk == false){
        this->control_clk = false;
        //cout << "Control nededge: " << this->id << endl;

        //Handle an Snoop if snoop_flag is true for this core
        if(snoop_flag->at(this->id)){

            //cout <<"ENTRO A SNOOPFLAG"<<endl;
            
            snoop_flag->at(this->id) = 0; //Message received

            //Get state of block_number = message address
            State state = this->states->at(actualMessage->getAddress());

                        //cout <<"SNOOP state "<<state<<endl;

                        //cout <<"SNOOP type "<<actualMessage->getType()<<endl;

                        //cout <<"SNOOP addr "<<actualMessage->getAddress()<<endl;


            switch(state){
                case Control::INVALID:

                    //The read of this core is being processed
                    if(actualMessage->getType() == 0 && actualMessage->getId() == this->id){ 
                        
                        cout<<"Control:"<<this->id<<"READ BEING POROCESSED"<<endl;

                        //Update block state
                        //this->states->at(actualMessage->getAddress()) = SHARED;
                        //cout <<"Control:"<<this->id<<"++++++++++++++++++++++++Changed from invalid to shared++++++++++++++++++++++++++++++++++++ "<<endl;

                    }

                    //The write of this core is being processed
                    else if(actualMessage->getType() == 1 && actualMessage->getId() == this->id){ 

                        cout<<"Control:"<<this->id<<"WRITE BEING POROCESSED"<<endl;


                        //Update block state
                        //this->states->at(actualMessage->getAddress()) = MODIFIED;
                        //cout <<"Control:"<<this->id <<"++++++++++++++++++++++++Changed from invalid to modified++++++++++++++++++++++++++++++++++++ "<<endl;
                    }

                    //Read process finished
                    else if(actualMessage->getType() == 2 && actualMessage->getId() == this->id){ 

                        cout<<"Control:"<<this->id<<"READ READY"<<endl;

                        //Put data in cache
                        
                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();

                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = SHARED;
                        cout <<"Control:"<<this->id<<"++++++++++++++++++++++++Changed from invalid to shared++++++++++++++++++++++++++++++++++++ "<<endl;

                    }
                    //Write process finished
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 

                        cout<<"Control:"<<this->id<<"WRITE READY"<<endl;

                        //Put data in cache
                        
                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();


                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control:"<<this->id <<"++++++++++++++++++++++++Changed from invalid to modified++++++++++++++++++++++++++++++++++++ "<<endl;

                    }

                    //Snoop Read and Write from other cores doesnt change INVALID state 
                    
                    break;
                case Control::MODIFIED: 

                    //Snoop read from other core
                    if(actualMessage->getType() == 0 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = SHARED;
                    }

                    //Snoop write from other core
                    else if(actualMessage->getType() == 1 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;
                    }

                    //Write message received by bus
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 
                        cout<<"Control:"<<this->id<<"WRITE READY"<<endl;

                        //Put data in cache
                        
                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();


                        //Free controller unit
                        this->wait = false;
                    }

                    break;

                case Control::SHARED:

                    //The write of this core is being processed
                    if(actualMessage->getType() == 1 && actualMessage->getId() == this->id){ 

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control:"<<this->id <<"++++++++++++++++++++++++Changed from shared to modified++++++++++++++++++++++++++++++++++++ "<<endl;
                    }

                    //Write process finished
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 

                        cout<<"Control:"<<this->id<<"WRITE READY"<<endl;

                        //Put data in cache
                        
                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();


                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control:"<<this->id <<"++++++++++++++++++++++++Changed from shared to modified++++++++++++++++++++++++++++++++++++ "<<endl;

                    }

                    //Ready message received by bus
                    else if(actualMessage->getType() == 2 && actualMessage->getId() == this->id){ 
                        cout<<"Control:"<<this->id<<"Read ready"<<endl;

                        //Put data in cache
                        
                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();


                        //Free controller unit
                        this->wait = false;
                    }  

                   //Snoop write from other core
                    else if(actualMessage->getType() == 1 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;

                    }
                   
                    break;
            }
        }
     }
}


/**
 * @brief Get the Id object
 * 
 * @return int 
 */
int Control::getId(){
    return this->id;    
}
