#include "CacheBlock.h"

/**
 * @brief Construct a new Cache Block:: Cache Block object
 * 
 * @param tag block identifier
 * @param state Initial state
 * @param data Initial data
 */
CacheBlock::CacheBlock(int tag, CacheBlock::State state, int data){
    this->tag = tag;
    this->block_state = state;
    this->data = data;
}

/**
 * @brief Destroy the Cache Block:: Cache Block object
 * 
 */
CacheBlock::~CacheBlock(){}

/**
 * @brief Get the State object
 * 
 * @return State 
 */
string CacheBlock::getStateString(){
    switch(this->block_state){
        case INVALID:
            return "Invalid";
        break;
        case MODIFIED:
            return "Modified";
        break;
        case SHARED:
            return "Shared";
        break;
        case OWNER:
            return "Owner";
        break;
        case EXCLUSIVE:
            return "Exclusive";
        break;
    }
}

/**
 * @brief Get the State object
 * 
 * @return CacheBlock::State 
 */
CacheBlock::State CacheBlock::getState(){
    return this->block_state;
}

/**
 * @brief Get the Tag object
 * 
 * @return int 
 */
int CacheBlock::getTag(){
    return this->tag;
}

/**
 * @brief Get the Data object
 * 
 * @return int 
 */
int CacheBlock::getData(){
    return this->data;
}

void CacheBlock::setState(CacheBlock::State state){
    this->block_state = state;
}

/**
 * @brief Set the Tag object
 * 
 * @param tag 
 */
void CacheBlock::setTag(int tag){
    this->tag = tag;
}

/**
 * @brief Set the Data object
 * 
 * @param data 
 */
void CacheBlock::setData(int data){
    this->data = data;
}