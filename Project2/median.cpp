#include "median.h"

void median::push(order* new_order){
    
    if(min_heap.empty() && max_heap.empty()){
        min_heap.push(new_order);
    }
    else{
        if(min_heap.top()->get_price() >= new_order->get_price()){
            min_heap.push(new_order);
        }
        else{
            max_heap.push(new_order);
        }
    }
    if(abs(min_heap.size() - max_heap.size()) >= 2){
        if(min_heap.size() > max_heap.size()){
            order* swap = min_heap.top();
            min_heap.pop();
            max_heap.push(swap);
        }
        else{
            order* swap = max_heap.top();
            max_heap.pop();
            min_heap.push(swap);
        }
    }
    return;
}

long long median::get_median(){
    if(min_heap.size() == 0 && max_heap.size() == 0){
        return 0;
    }
    else if(min_heap.size() == 0){
        return max_heap.top()->get_price();
    }
    else if(max_heap.size() == 0){
        return min_heap.top()->get_price();
    }
    
    if(min_heap.size() > max_heap.size()){
        return min_heap.top()->get_price();
    }
    else if(max_heap.size() > min_heap.size()){
        return max_heap.top()->get_price();
    }
    else{
        order* min_top = min_heap.top();
        order* max_top = max_heap.top();
        long long median = min_top->get_price() + max_top->get_price();
        median = median / 2;
        return median;
    }
}