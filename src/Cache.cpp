#include "Cache.h"

/**
 * @brief Construct a new Cache:: Cache object
 * 
 * @param id Cache identifier
 */
Cache::Cache(int id){

    this->data = -1;
    this->address = -1;
    this->cache_clk = false;
    this->miss = false;
    this->hit = false;
    this->read_flag = false;
    this->write_flag = false;
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
 * @param memoryBlock 
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
       cout << "||  " <<memoryBlocks->at(i)->getTag() << "  || " << memoryBlocks->at(i)->getState() << " ||  " << memoryBlocks->at(i)->getData() << "  ||" <<endl;
    }

}