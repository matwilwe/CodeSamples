#include "market_helper.h"

bool check_string(const std::string &check){
    
    for(unsigned int i = 0; i < check.size(); ++i){
        if(isalnum(check[i]) == 0 && check[i] != '_'){
            return false;
        }
    }
    return true;
}

unsigned int check_insider(const std::vector<std::string> &insiders, const std::string &current_client){
    for(unsigned int i = 0; i < insiders.size(); ++i){
        if(insiders[i] == current_client){
            return i;
        }
    }
    return insiders.size() + 1;
}



std::pair<std::vector<order*>::const_iterator, std::vector<order*>::const_iterator> time_traveler(const std::vector<order*> &ttt_arr){
    std::vector<order*>::const_iterator first, second, closest1, closest2;
    first = ttt_arr.begin();
    second = ttt_arr.begin();
    closest1 = ttt_arr.end();
    closest2 = ttt_arr.end();
    long long largest_diff = 0;
   
    while((*second)->get_buy() == true){
        ++second;
    }
    first = second;
    ++second;
    while(second != ttt_arr.end()){
        if((*second)->get_buy() == false){
            if((*second)->get_price() < (*first)->get_price()){
                first = second;
            }
        }
        else{
            if((*second)->get_price() - (*first)->get_price() > largest_diff || (closest1 == ttt_arr.end() && closest2 == ttt_arr.end())){
                closest1 = first;
                closest2 = second;
                largest_diff = (*closest2)->get_price() - (*closest1)->get_price();
            }
        }
        ++second;
    }
    return std::pair <std::vector<order*>::const_iterator, std::vector<order*>::const_iterator> (closest1, closest2);
}
