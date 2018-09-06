/**
 * @brief BusMessage model
 * 
 * @file BusMessage.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#include "BusMessage.h"

/**
 * @brief Construct a new Bus Message:: Bus Message object
 * 
 * @param id core that created the message
 * @param type 0 = RD | 1 = WR | 2 = READY RD | 3 = READY WR
 * @param address 
 * @param data 
 */
BusMessage::BusMessage(int id, int type, int address, int data){
    this->id = id;
    this->type = type;
    this->address = address;
    this->data = data;
}

/**
 * @brief Destroy the Bus Message:: Bus Message object
 * 
 */
BusMessage::~BusMessage(){}

/**
 * @brief Set id
 * 
 * @param id 
 */
void BusMessage::setId(int id){
    this->id = id;
}

/**
 * @brief Set type
 * 
 * @param type 
 */
void BusMessage::setType(int type){
    this->type = type;
}

/**
 * @brief Set address
 * 
 * @param address 
 */
void BusMessage::setAddress(int address){
    this->address = address;
}

/**
 * @brief Set data
 * 
 * @param data 
 */
void BusMessage::setData(int data){
    this->data = data;
}

/**
 * @brief Get id
 * 
 * @return int 
 */
int BusMessage::getId(){
    return this->id;
}

/**
 * @brief Get type
 * 
 * @return int 
 */
int BusMessage::getType(){
    return this->type;
}

/**
 * @brief Get address
 * 
 * @return int 
 */
int BusMessage::getAddress(){
    return this->address;
}

/**
 * @brief Get data
 * 
 * @return int 
 */
int BusMessage::getData(){
    return this->data;
}