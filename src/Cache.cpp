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
        CacheBlock* cacheBlock = new CacheBlock(i,0);
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
 * @brief Get the Data object
 * 
 * @return vector<int> 
 */
vector<int> Cache::getData(){
    vector<int> data;
    for(int i = 0; i < 16; i++){
        data.push_back(memoryBlocks->at(i)->getData());
    }

    return data;
}

/**
 * @brief Update cache every clock
 * 
 * @param clk 
 * @param data 
 * @param address 
 * @param write_flag 
 * @param read_flag 
 * @param snoop_flag_cache 
 * @param snoop_data_cache 
 * @param snoop_address_cache 
 */
void Cache::loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache){
    //Posedge
    if(this->cache_clk == false && clk == true){
        this->cache_clk = true;
        //cout<<"Cache: "<< this->id <<endl;        

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
        //cout<<"Cache nededge: "<< this->id <<endl; 

        //Read data in memory block for snoop handle
        if(snoop_flag_cache){
            snoop_flag_cache = 0;
             this->memoryBlocks->at(snoop_address_cache)->setData(snoop_data_cache);
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
