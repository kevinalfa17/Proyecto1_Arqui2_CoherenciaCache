/**
 * @brief Model of message handled by bus and used by other components
 * 
 * @file BusMessage.h
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#ifndef BUS_MESSAGE_H
#define BUS_MESSAGE_H

class BusMessage{

private:
    int id;
    int type;
    int address;
    int data;
    
public:
    BusMessage(int id, int type, int address, int data);
    ~BusMessage();

    void setId(int id);
    void setType(int type);
    void setAddress(int address);
    void setData(int data);

    int getId();
    int getType();
    int getAddress();
    int getData();
    
};

#endif