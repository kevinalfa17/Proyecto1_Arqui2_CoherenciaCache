/**
 * @brief Cache block model
 * 
 * @file CacheBlock.h
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#ifndef CACHE_BLOCK_H
#define CACHE_BLOCK_H

#include <string>
using namespace std;

class CacheBlock{
    
public:

    CacheBlock(int tag, int data);
    ~CacheBlock();

    int getTag();
    int getData();
    void setTag(int tag);
    void setData(int data);

private:
    int tag;
    int data;
   
};

#endif