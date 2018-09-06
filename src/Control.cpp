/**
 * @brief Control unit model
 * 
 * @file Control.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

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
 * @brief Return states in string format inside vector
 * 
 * @return vector<string> 
 */
vector<string> Control::getStates(){
    vector<string> states_vector;

    for(int i = 0; i < 16; i++){
        switch(this->states->at(i)){
            case INVALID:
                states_vector.push_back("INVALID");
            break;
            case MODIFIED:
                states_vector.push_back("MODIFIED");
            break;
            case SHARED:
                states_vector.push_back("SHARED");
            break;
        }
    }

    return states_vector;

}


/**
 * @brief Update control unit every clock
 * 
 * @param clk 
 * @param data 
 * @param address 
 * @param write_flag_cpu 
 * @param read_flag_cpu 
 * @param write_flag_cache 
 * @param read_flag_cache 
 * @param ready 
 * @param snoop_flag 
 * @param queue 
 * @param actualMessage 
 * @param snoop_flag_cache 
 * @param snoop_data_cache 
 * @param snoop_address_cache 
 * @param busEnable 
 * @param cacheEnable 
 */
void Control::loop(bool clk,int & data, int & address, bool & write_flag_cpu, bool & read_flag_cpu, bool & write_flag_cache, bool & read_flag_cache, bool & ready, vector<bool> * snoop_flag,  vector<BusMessage*> * queue, BusMessage * actualMessage, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache, bool & busEnable, bool & cacheEnable){


    //Posedge
    if(this->control_clk == false && clk == true){
        this->control_clk = true;
        //cout<<"Control unit: "<< this->id <<endl;
        int state;
        //Read data in memory block
        if(read_flag_cpu && !this->wait && !ready){


            //Get state of block_number = address
            state = this->states->at(address);

            cout <<"\n****************Se hará un READ en el CPU"<<this->id<<" a la direccion: "<<address<<"***************"<<endl;


            switch(state){
                case Control::INVALID:
                    cout << "****************INVALID: Read Miss in cache: CPU" << this->id <<"******************"<<endl;

                    //Put read message in bus queue
                    BusMessage * message;
                    message = new BusMessage(this->id,0,address,data);
                    queue->push_back(message);

                    this->wait = true; //This variable avoid to repeat this process if message was already sent to bus

                    break;
                case Control::MODIFIED:
                    //HIT
                    cout << "****************MODIFIED: Read Hit in cache: CPU" << this->id <<"******************"<<endl;

                    //Traer dato de cache
                    read_flag_cache = true;

                    //Tell processor data is ready
                    ready = true;

                    //Free controller unit
                    this->wait = false;

                    
                    break;
                case Control::SHARED:
                    //HIT
                    cout << "****************SHARED: Read Hit in cache: CPU" << this->id <<"******************"<<endl;
                    
                    //Traer dato de cache
                    read_flag_cache = true;

                    //Tell processor data is ready
                    ready = true;

                    //Free controller unit
                    this->wait = false;
                    break;
            }
            
        }
        //Write data in memory block
        else if(write_flag_cpu && !this->wait && !ready){


            //Get state of block_number = address
            state = this->states->at(address);

            cout <<"\n****************Se hará un WRITE en el CPU"<<this->id<<" A la direccion: "<<address<<" Dato: "<<data<<"***************"<<endl;

            switch(state){
                case Control::INVALID:
                    cout << "****************INVALID: Write Miss in cache: CPU" << this->id <<"******************"<< endl;
                    break;
                case Control::MODIFIED:
                    cout << "*****************MODIFIED: Write Hit in cache, write through: CPU" << this->id <<"******************"<<  endl;
                    break;
                case Control::SHARED:
                     cout << "****************MODIFIED: Write Hit in cache, write through: CPU" << this->id <<"******************"<<  endl;
                    break;
            }
            
            //Put write message in bus queue
            BusMessage * message;
            message = new BusMessage(this->id,1,address,data);
            queue->push_back(message);
            this->wait = true;
                    
        }

        busEnable = true;
        cacheEnable = true;
    }
    //Nededge
    else if(this->control_clk == true && clk == false){
        this->control_clk = false;
        //cout << "Control nededge: " << this->id << endl;

        //Handle an Snoop if snoop_flag is true for this core
        if(snoop_flag->at(this->id)){
            
            snoop_flag->at(this->id) = false; //Message received

            //Get state of block_number = message address
            State state = this->states->at(actualMessage->getAddress());

            switch(state){
                case Control::INVALID:

                    //The read of this core is being processed
                    if(actualMessage->getType() == 0 && actualMessage->getId() == this->id){ 
                        
                        cout<<"Control: "<<this->id<<" READ BEING PROCESSED"<<endl;

                    }

                    //The write of this core is being processed
                    else if(actualMessage->getType() == 1 && actualMessage->getId() == this->id){ 

                        cout<<"Control: "<<this->id<<" WRITE BEING PROCESSED"<<endl;

                    }

                    //Read process finished
                    else if(actualMessage->getType() == 2 && actualMessage->getId() == this->id){ 

                        cout<<"Control: "<<this->id<<" READ READY"<<endl;

                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();
                        
                        //Put data in cache
                        snoop_flag_cache = true;
                        snoop_data_cache = data;
                        snoop_address_cache = actualMessage->getAddress();

                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = SHARED;
                        cout <<"Control: "<<this->id<<"++++++++++++++++++++++++Changed from INVALID to SHARED++++++++++++++++++++++++++++++++++++ "<<endl;

                    }
                    //Write process finished
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 

                        cout<<"Control: "<<this->id<<" WRITE READY"<<endl;

                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();
                        
                        //Put data in cache
                        snoop_flag_cache = true;
                        snoop_data_cache = data;
                        snoop_address_cache = actualMessage->getAddress();
                        

                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control: "<<this->id <<"++++++++++++++++++++++++Changed from INVALID to MODIFIED++++++++++++++++++++++++++++++++++++ "<<endl;

                    }

                    //Snoop Read and Write from other cores doesnt change INVALID state 
                    
                    break;
                case Control::MODIFIED: 

                    //Snoop read from other core
                    if(actualMessage->getType() == 0 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = SHARED;
                    }
                    //Snoop read from other core
                    else if(actualMessage->getType() == 2 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = SHARED;
                    }

                    //Snoop write from other core
                    else if(actualMessage->getType() == 1 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;
                    }

                    //Snoop write from other core
                    else if(actualMessage->getType() == 3 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;

                    }

                    //Write message received by bus
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 
                        cout<<"Control: "<<this->id<<" WRITE READY"<<endl;

                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();
                        
                        //Put data in cache
                        snoop_flag_cache = true;
                        snoop_data_cache = data;
                        snoop_address_cache = actualMessage->getAddress();

                        //Free controller unit
                        this->wait = false;
                    }

                    break;

                case Control::SHARED:

                    //The write of this core is being processed
                    if(actualMessage->getType() == 1 && actualMessage->getId() == this->id){ 

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control: "<<this->id <<"++++++++++++++++++++++++Changed from SHARED to MODIFIED++++++++++++++++++++++++++++++++++++ "<<endl;
                    }

                    //Write process finished
                    else if(actualMessage->getType() == 3 && actualMessage->getId() == this->id){ 

                        cout<<"Control: "<<this->id<<" WRITE READY"<<endl;

                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();
                        
                        //Put data in cache
                        snoop_flag_cache = true;
                        snoop_data_cache = data;
                        snoop_address_cache = actualMessage->getAddress();

                        //Free controller unit
                        this->wait = false;

                        //Update block state
                        this->states->at(actualMessage->getAddress()) = MODIFIED;
                        cout <<"Control: "<<this->id <<"++++++++++++++++++++++++Changed from SHARED to MODIFIED++++++++++++++++++++++++++++++++++++ "<<endl;

                    }

                    //Ready message received by bus
                    else if(actualMessage->getType() == 2 && actualMessage->getId() == this->id){ 
                        cout<<"Control: "<<this->id<<" READ READY"<<endl;

                        //Tell processor read is ready
                        ready = true;
                        data = actualMessage->getData();
                        
                        //Put data in cache
                        snoop_flag_cache = true;
                        snoop_data_cache = data;
                        snoop_address_cache = actualMessage->getAddress();

                        //Free controller unit
                        this->wait = false;
                    }  

                   //Snoop write from other core
                    else if(actualMessage->getType() == 1 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;

                    }
                    //Snoop write from other core
                    else if(actualMessage->getType() == 3 && actualMessage->getId() != this->id){ 
                        //Update block state
                        this->states->at(actualMessage->getAddress()) = INVALID;
                        cout<<"*********Control: "<<this->id<<" INVALIDATED block:" << actualMessage->getAddress() <<"*********"<<endl;

                    }
                   
                    break;
            }
        }
        busEnable = true;
        cacheEnable = true;
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
