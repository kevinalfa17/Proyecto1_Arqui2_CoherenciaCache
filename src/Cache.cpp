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
Cache::~Cache(){
    delete this->memoryBlocks;
}

/**
 * @brief Create memory structure of 16 Cache blocks
 * 
 * @param memoryBlocks
 */
void Cache::initMemory(){
    this->memoryBlocks = new vector<CacheBlock*>();
    for(int i = 0; i < 16; i++){
        CacheBlock* cacheBlock = new CacheBlock(i,i+100);
        this->memoryBlocks->push_back(cacheBlock);
    }
}

/**
 * @brief Print a table with all cache tags and data
 * 
 */
void Cache::printMemory(){
    cout<<"|| TAG || DATA ||"<<endl;
    for(int i = 0; i < 16; i++){
       cout << "||  " <<memoryBlocks->at(i)->getTag() << "  || " << memoryBlocks->at(i)->getData() << "  ||" <<endl;
    }
}

/**
 * @brief Update cache in every clock cycle
 * 
 * @param clk Global clock
 * @param data data from/to control unit
 * @param address address from/to control unit
 * @param write_flag true if control unit needs to write
 * @param read_flag true if control unit needs to read
 * @param snoop_flag true if control unit will update state after snoop 
 */
void Cache::loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache){
    //Posedge
    if(this->cache_clk == false && clk == true){
        this->cache_clk = true;
        cout<<"Cache: "<< this->id <<endl;        

        //Read data in memory block
        if(read_flag){
            read_flag = false;
            data = this->memoryBlocks->at(address)->getData();
        }
        //Write data in memory block
        else if(write_flag){
            write_flag = false;
            this->memoryBlocks->at(address)->setData(data);
        }
    }
    //Nededge
     if(this->cache_clk == true && clk == false){
        this->cache_clk = false;
        cout<<"Cache nededge: "<< this->id <<endl; 

        //Read data in memory block for snoop handle
        if(snoop_flag_cache){
            snoop_flag_cache = 0;
            snoop_data_cache = this->memoryBlocks->at(snoop_address_cache)->getData();
        }
    
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
