#ifndef CACHE_H
#define CACHE_H

#include "CacheBlock.h"
#include <vector>
#include <iostream>

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
    void loop(bool clk,int & data, int & address, bool & write_flag, bool & read_flag, bool & snoop_flag_cache, int & snoop_data_cache, int & snoop_address_cache);
    int getId();
};

#endif