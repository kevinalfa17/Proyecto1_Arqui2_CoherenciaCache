/**
 * @brief Cache block model
 * 
 * @file CacheBlock.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#include "CacheBlock.h"

/**
 * @brief Construct a new Cache Block:: Cache Block object
 * 
 * @param tag block identifier
 * @param data Initial data
 */
CacheBlock::CacheBlock(int tag, int data){
    this->tag = tag;
    this->data = data;
}

/**
 * @brief Destroy the Cache Block:: Cache Block object
 * 
 */
CacheBlock::~CacheBlock(){}


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