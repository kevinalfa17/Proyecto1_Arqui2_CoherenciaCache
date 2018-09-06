/**
 * @brief Clock model
 * 
 * @file Clock.cpp
 * @author Kevin Alfaro
 * @date 2018-09-05
 */

#include "Clock.h"

/**
 * @brief Construct a new Clock object
 * 
 */
Clock::Clock(){
    this->cycle_counter = 0;
}

/**
 * @brief Destroy the Clock object
 * 
 */
Clock::~Clock(){}
    
/**
 * @brief Alternates global clock
 * 
 * @param clk 
 */
void Clock::loop(bool & clk){
    
    clk = !clk;
    if(clk){
        cout<<"---------------------------Posedge: Cycle: "<<cycle_counter<<"----------------------------------"<<endl;
        cycle_counter++;
    }
    else{
        cout<<"---------------------------Nededge: Cycle: "<<cycle_counter<<"----------------------------------"<<endl;
    }

}