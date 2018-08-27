#ifndef CACHE_H
#define CACHE_H

#include "CacheBlock.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Cache{

private:
    int id;
    bool cache_clk;
    vector<CacheBlock*>* memoryBlocks;

    void initMemory();

    
    
public:
    Cache(int id);
    ~Cache();

    void printMemory();
    void loop(bool clk,int & data, int & address, CacheBlock::State & state, bool & write_flag, bool & read_flag, bool & snoop_flag, bool & state_flag);
    int getId();
    CacheBlock::State getBlockState(int block_tag);
};

#endif