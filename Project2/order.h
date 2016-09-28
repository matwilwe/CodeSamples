#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <sstream>

class order{
    
private:
    bool buy;
    long long ID, timestamp, price, quantity;
    std::string client_name, equity_symbol;
    
public:
    order() : buy(false), ID(0), timestamp(0), price(0), quantity(0) {};
    
    order(bool buy_in, long long ID_in, long long timestamp_in, long long price_in, long long quantity_in, std::string client_in,
          std::string equity_in) : buy(buy_in), ID(ID_in), timestamp(timestamp_in), price(price_in),
    quantity(quantity_in), client_name(client_in), equity_symbol(equity_in) {};
    
    bool get_buy() const {return buy;};
    long long get_ID() const {return ID;};
    long long get_timestamp() const {return timestamp;};
    long long get_price() const{return price;};
    long long get_quantity() const {return quantity;};
    std::string get_client() const {return client_name;};
    std::string get_equity() const {return equity_symbol;};
    
    //Requires: buyer is a valid pointer
    //Modifies: buyer, os
    //Effects: processes the trade given the buy order and prints out the correct
    //information to os.
    long long buy_stock(order * buyer, bool verbose_print, std::ostringstream &os);

    
};

class buy_compare{
    
public:
    bool operator() (order* left, order* right) const{
        if(left->get_price() < right->get_price()){
            return true;
        }
        else if(right->get_price() < left->get_price()){
            return false;
        }
        else if(left->get_ID() > right->get_ID()){
            return true;
        }
        else{
            return false;
        }
    }
};


class sell_compare{
    
public:
    bool operator() (order* left, order* right) const{
        if(left->get_price() > right->get_price()){
            return true;
        }
        else if(right->get_price() > left->get_price()){
            return false;
        }
        else if(left->get_ID() > right->get_ID()){
            return true;
        }
        else{
            return false;
        }
    }
    
};

class price_compare_max{
    
public:
    bool operator() (order* left, order* right) const{
        if(left->get_price() > right->get_price()){
            return true;
        }
        else{
            return false;
        }
    }
    
};

class price_compare_min{
    
public:
    bool operator() (order* left, order* right) const{
        if(right->get_price() > left->get_price()){
            return true;
        }
        else{
            return false;
        }
    }
};
#endif
