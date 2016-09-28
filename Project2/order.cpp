#include "order.h"

long long order::buy_stock(order* seller, bool verbose_print, std::ostringstream &os){
    long long set_price = 0;
    if(this->get_ID() < seller->get_ID()){
        set_price = this->get_price();
    }
    else{
        set_price = seller->get_price();
    }
    long long total = 0;
    if(this->get_quantity() <= seller->get_quantity()){
        total = set_price * this->get_quantity();
        seller->quantity = seller->quantity - this->quantity;
        if(verbose_print){
            os << this->get_client() << " purchased " << this->get_quantity();
            os << " shares of " << this->get_equity() << " from " << seller->get_client();
            os << " for $" << set_price << "/share\n";
        }
        this->quantity = 0;
    }
    else{
        total = set_price * seller->get_quantity();
        this->quantity = this->quantity - seller->quantity;
        if(verbose_print){
            os << this->get_client() << " purchased " << seller->get_quantity();
            os << " shares of " << this->get_equity() << " from " << seller->get_client();
            os << " for $" << set_price << "/share\n";
        }
        seller->quantity = 0;
    }
    
    return total;
}
