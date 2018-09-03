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

BusMessage::~BusMessage(){}

void BusMessage::setId(int id){
    this->id = id;
}

void BusMessage::setType(int type){
    this->type = type;
}

void BusMessage::setAddress(int address){
    this->address = address;
}

void BusMessage::setData(int data){
    this->data = data;
}

int BusMessage::getId(){
    return this->id;
}

int BusMessage::getType(){
    return this->type;
}

int BusMessage::getAddress(){
    return this->address;
}

int BusMessage::getData(){
    return this->data;
}