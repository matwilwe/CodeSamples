#ifndef MEDIAN_H
#define MEDIAN_H
#include "order.h"
#include <queue>

class median{
    
private:
    std::priority_queue<order*, std::vector<order*>, price_compare_min> min_heap;
    std::priority_queue<order*, std::vector<order*>, price_compare_max> max_heap;
    
public:
    
    median()  {};
    
    //Requires: new_order is a valid pointer to an order.
    //Modifies: min_heap, max_heap
    //Effects: places new_order in the correct place in order to find the median of all orders
    //contained in the min_heap and max_heap.
    void push(order* new_order);
    
    //Effects: returns the long long value of the price of the median order.
    long long get_median();
    
};

#endif
