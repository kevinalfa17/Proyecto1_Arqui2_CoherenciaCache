#ifndef CACHE_BLOCK_H
#define CACHE_BLOCK_H

#include <string>
using namespace std;

class CacheBlock{
    
public:
    enum State{INVALID,MODIFIED,SHARED,OWNER,EXCLUSIVE};

    CacheBlock(int tag, State state, int data);
    ~CacheBlock();

    string getStateString();
    State getState();
    int getTag();
    int getData();
    void setState(State state);
    void setTag(int tag);
    void setData(int data);

private:
    int tag;
    int data;
    State block_state;
   
};

#endif