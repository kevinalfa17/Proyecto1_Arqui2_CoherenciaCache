#include "Cache.h"

/**
 * @brief Construct a new Cache:: Cache object
 * 
 * @param id Cache identifier
 */
Cache::Cache(int id){
    this->id = id;
    this->cache_clk = false;
    this->initMemory();
}

/**
 * @brief Destroy the Cache:: Cache object
 * 
 */
Cache::~Cache(){}

/**
 * @brief Create memory structure of 16 Cache blocks
 * 
 * @param memoryBlocks
 */
void Cache::initMemory(){
    this->memoryBlocks = new vector<CacheBlock*>();
    for(int i = 0; i < 16; i++){
        CacheBlock* cacheBlock = new CacheBlock(i,CacheBlock::INVALID,0);
        this->memoryBlocks->push_back(cacheBlock);
    }
}

/**
 * @brief Print a table with all cache tags, state and data
 * 
 */
void Cache::printMemory(){
    cout<<"|| TAG ||  STATE  || DATA ||"<<endl;
    for(int i = 0; i < 16; i++){
       cout << "||  " <<memoryBlocks->at(i)->getTag() << "  || " << memoryBlocks->at(i)->getStateString() << " ||  " << memoryBlocks->at(i)->getData() << "  ||" <<endl;
    }
}

/**
 * @brief Update cache in every clock cycle
 * 
 * @param clk Global clock
 * @param data data from/to control unit
 * @param address address from/to control unit
 * @param state state of block from/to control unit
 * @param write_flag true if control unit needs to write
 * @param read_flag true if control unit needs to read
 * @param snoop_flag true if control unit will update state after snoop 
 * @param state_flag true if control unit needs block state with tag = address
 */
void Cache::loop(bool clk,int & data, int & address,CacheBlock::State & state, bool & write_flag, bool & read_flag, bool & snoop_flag, bool & state_flag){
    //Posedge
    if(this->cache_clk == false && clk == true){
        this->cache_clk = true;
        cout<<"Cache: "<< this->id <<endl;

        //Send state to control unit
        if(state_flag){
            state = this->memoryBlocks->at(address)->getState();
        }

        //Read data in memory block
        if(read_flag){
            
        }
        //Write data in memory block
        else if(write_flag){
        
        }
        //Update state caused by snoop
        else if(snoop_flag){

        }
    }
    //Nededge
     if(this->cache_clk == true && clk == false){
        this->cache_clk = false;
     }
}

/**
 * @brief Get the Id object
 * 
 * @return int 
 */
int Cache::getId(){
    return this->id;    
}

/**
 * @brief Get block state inside the memory blocks
 * 
 * @param block_tag block address
 * @return CacheBlock::State 
 */
CacheBlock::State Cache::getBlockState(int block_tag){
    return this->memoryBlocks->at(block_tag)->getState();
}