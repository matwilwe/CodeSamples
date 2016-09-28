#include "transfer.h"
void transfer::stock_sold(order* seller, order* buyer){
    long long price = 0;
    long long stocks_sold = 0;
    if(seller->get_ID() > buyer->get_ID()){
        price = buyer->get_price();
    }
    else{
        price = seller->get_price();
    }
    
    if(seller->get_quantity() >= buyer->get_quantity()){
        stocks_sold = buyer->get_quantity();
    }
    else{
        stocks_sold = seller->get_quantity();
    }
    
    number_stocks_sold = number_stocks_sold + stocks_sold;
    total = total + price * stocks_sold;
    
}

void transfer::stock_bought(order* seller, order* buyer){
    
    long long price = 0;
    long long stocks_bought = 0;
    if(seller->get_ID() > buyer->get_ID()){
        price = buyer->get_price();
    }
    else{
        price = seller->get_price();
    }
    
    if(seller->get_quantity() >= buyer->get_quantity()){
        stocks_bought = buyer->get_quantity();
    }
    else{
        stocks_bought = seller->get_quantity();
    }
    
    number_stocks_bought = number_stocks_bought + stocks_bought;
    total = total - price * stocks_bought;
}

void transfer::stock_sold_insider(order* buyer){
    
    number_stocks_sold = number_stocks_sold + buyer->get_quantity();
    total = total + buyer->get_quantity() * buyer->get_price();
    return;
}

void transfer::stock_bought_insider(order* seller){
    
    number_stocks_bought = number_stocks_bought + seller->get_quantity();
    total = total - seller->get_quantity() * seller->get_price();
    return;
}

void transfer::stock_to_insider(order* seller){
    
    number_stocks_sold = number_stocks_sold + seller->get_quantity();
    total = total + seller->get_quantity() * seller->get_price();
    return;
    
}

void transfer::stock_from_insider(order* buyer){
    
    number_stocks_bought = number_stocks_bought + buyer->get_quantity();
    total = total - buyer->get_quantity() * buyer->get_price();
    return;
    
}
