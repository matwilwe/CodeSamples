#ifndef TRANSFER_H
#define TRANSFER_H
#include "order.h"

class transfer{
    
private:
    long long number_stocks_bought;
    long long number_stocks_sold;
    long long total;
    
public:
    transfer() : number_stocks_bought(0), number_stocks_sold(0), total(0) {}
    
    //Requires: seller and buyer are valid orders.
    //modifies: number_stocks_sold, total
    //effects: updates the totals for the client who is selling.
    void stock_sold(order* seller, order* buyer);
    
    //Requires: seller and buyer are valid pointers.
    //modifies: number_stocks_bought, total
    //effecs: updates the totals for the buying client.
    void stock_bought(order* seller, order* buyer);
    
    //Requires: seller is a valid pointer
    //modifies: number_stocks bought, total.
    //effects: updates the totals for the insider.
    void stock_bought_insider(order* seller);
    
    //Requires: buyer is a valid pointer
    //modifies: number_stocks_sold, total.
    //effects: updates the totals for the insider.
    void stock_sold_insider(order* buyer);
    
    //Requires: seller is a valid pointer
    //modifies: number_stocks_bought, total/
    //effects: updates the total for the insider.
    void stock_to_insider(order* seller);
    
    //Requires: seller is a valid pointer
    //modifies: number_stocks_sold, total
    //effects: updates the total for the insider.
    void stock_from_insider(order* buyer);
    
    long long get_number_bought() const {return number_stocks_bought;}
    long long get_number_sold() const {return number_stocks_sold;}
    long long get_net_transfer() const {return total;}
    
};

#endif
