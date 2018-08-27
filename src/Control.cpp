#include "Control.h"

/**
 * @brief Construct a new Control:: Control object
 * 
 * @param id Control identifier
 */
Control::Control(int id){
    this->id = id;
    this->control_clk = false;
}

/**
 * @brief Destroy the Control:: Control object
 * 
 */
Control::~Control(){}

/**
 * @brief Update control unit in every cycle
 * 
 * @param clk Global clock
 * @param data_cpu data from/to cpu
 * @param address_cpu address from/to cpu
 * @param write_flag_cpu true if cpu needs to write
 * @param read_flag_cpu true if cpu needs to read
 * @param data_cache data from/to cache
 * @param address_cache address from/to cache
 * @param write_flag_cache true if control unit needs to write in the cache
 * @param read_flag_cache true if cpu needs to write in the cache
 * @param snoop_flag_cache true if control unit need to update cache state after snoop
 * @param snoop_flag_bus true if control unit needs to know state of tag = address
 */
void Control::loop(bool clk,int & data_cpu, int & address_cpu, bool & write_flag_cpu, bool & read_flag_cpu,int & data_cache, int & address_cache, bool & write_flag_cache, bool & read_flag_cache, bool & snoop_flag_cache, bool & snoop_flag_bus, CacheBlock::State & state){

    //Posedge
    if(this->control_clk == false && clk == true){
        this->control_clk = true;
        cout<<"Control unit: "<< this->id <<endl;

        //Read data in memory block
        if(read_flag_cpu){

            switch(state){
                case CacheBlock::INVALID:
                //PRINT MISS
                //PUT RD ON CONTROLER -> BUS (BUS CHECK IF DIRECTION IS IN ANOTHER CACHE)
                //WAIT UNTIL BUS TAKES DATA FROM MEMORY OR ANOTHER CACHE
                //IF IT WAS A CACHE OWNER RESPONSE --> SHARED
                //IF IT WAS A MEMORY --> EXCLUSIVE
                //PUT DATA ON ADDRESS
                //PRINT READY
                break;
                case CacheBlock::MODIFIED:
                //HIT
                
                break;
                case CacheBlock::SHARED:
                //HIT
                break;
                case CacheBlock::OWNER:
                //HIT
                
                break;
                case CacheBlock::EXCLUSIVE:
                //HIT
                break;
            }
        }
        //Write data in memory block
        else if(write_flag_cpu){
        
            switch(state){
                case CacheBlock::INVALID:
                //PRINT MISS
                //PUT WR ON CONTROLER -> BUS (BUS INVALIDATE )
                //WAIT UNTIL INVALIDATE FINISHED (READY)
                //STATE = MODIFIED
                //PRINT READY
                break;
                case CacheBlock::MODIFIED:
                //HIT
                break;
                case CacheBlock::SHARED:
                //PRINT INVALIDATE PROCESS
                //PUT WR ON CONTROLER -> BUS (BUS INVALIDATE )
                //WAIT UNTIL INVALIDATE FINISHED (READY)
                //STATE = MODIFIED
                //PRINT READY
                
                break;
                case CacheBlock::OWNER:
                //PRINT INVALIDATE PROCESS
                //PUT WR ON CONTROLER -> BUS (BUS INVALIDATE )
                //WAIT UNTIL INVALIDATE FINISHED (READY)
                //STATE = MODIFIED
                //PRINT READY
                
                break;
                case CacheBlock::EXCLUSIVE:
                //HIT
                break;
            }
            
        }

        //Handle an Snoop
        else if(snoop_flag_bus){
            switch(state){
                case CacheBlock::INVALID:
                //PRINT MISS
                //PUT RD ON CONTROLER -> BUS (BUS CHECK IF DIRECTION IS IN ANOTHER CACHE)
                //WAIT UNTIL BUS TAKES DATA FROM MEMORY OR ANOTHER CACHE
                //IF IT WAS A CACHE OWNER RESPONSE --> SHARED
                //IF IT WAS A MEMORY --> EXCLUSIVE
                //PUT DATA ON ADDRESS
                //PRINT READY
                break;
                case CacheBlock::MODIFIED:
                //HIT
                
                break;
                case CacheBlock::SHARED:
                //HIT
                break;
                case CacheBlock::OWNER:
                //HIT
                
                break;
                case CacheBlock::EXCLUSIVE:
                //HIT
                break;
            }
        }
    }
    //Nededge
     if(this->control_clk == true && clk == false){
        this->control_clk = false;
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
