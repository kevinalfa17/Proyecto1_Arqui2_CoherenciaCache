#ifndef CACHE_H
#define CACHE_H

#include "CacheBlock.h"
#include <vector>
#include <iostream>

using namespace std;

class Cache{

private:
    int id;
    int data;
    int address;
    bool cache_clk;
    bool miss;
    bool hit;
    bool read_flag;
    bool write_flag;
    vector<CacheBlock*>* memoryBlocks;

    void initMemory();

    
    
public:
    Cache(int id);
    ~Cache();

    void printMemory();
};

#endif