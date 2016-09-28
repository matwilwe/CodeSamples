#include <getopt.h>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "market_helper.h"
#include <random>
#include "median.h"
#include <map>
#include <utility>
#include "transfer.h"
#include <sstream>
#include <iostream>

using namespace std;


int main(int argc, char **argv){
  //Declare variables.
    unordered_map<string, priority_queue<order*, vector<order*>, sell_compare>> sell_book;
    unordered_map<string, priority_queue<order*, vector<order*>, buy_compare>> buy_book;
    map<string, class median> median_tracker;
    map<string, transfer> transfer_tracker;
    vector<order*> clean_up;
    vector<string> insider_equities, ttt_equities;
    bool summary = false, verbose = false, median_true = false, transfer_true = false;
    vector < vector <order*> > ttt_orders;
    ostringstream out;
    ios_base::sync_with_stdio(false);
    //----------------------------------GETOPT-------------------------------------
    
    struct option longOpts[] = {
        {"summary", no_argument, NULL, 's'},
        {"verbose", no_argument, NULL, 'v'},
        {"median", no_argument, NULL, 'm'},
        {"transfers", no_argument, NULL, 't'},
        {"insider", required_argument, NULL, 'i'},
        {"ttt", required_argument, NULL, 'g'},
        {"help", no_argument, NULL, 'h'}
    };
    int opt = 0, index = 0;
    while((opt = getopt_long(argc, argv, "svmti:g:h", longOpts, &index)) != -1){
        
        switch(opt){
            case 's':
                summary = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'm':
                median_true = true;
                break;
            case 't':
                transfer_true = true;
                break;
            case 'i':
                insider_equities.push_back(optarg);
                break;
            case 'g':
                ttt_equities.push_back(optarg);
                break;
            case 'h':
                cout << "Name\n  market - a command line stock exchange simulation program.\n"
                << "Synopsis\n  market (-s | --summary) SUMMARY (-v | --verbose) VERBOSE\n"
                << "  (-m | --median) MEDIAN (-t | --transfers) TRANSFERS (i | --insider\n"
                << "  EQUITY_SYMBOL) INSIDER (-g | -ttt EQUITY_SYMBOL) TIMETRAVELTRADING\n"
                << "  (-h | --help) HELP\n"
                << "Description\n  Stock Market simulator where any market client can place a buy\n"
                << "  or sell order for an equity.  This program will match buyers will sellers as\n"
                << "  quickly as possible."
                << "Options\n"
                << "  -s, --summary\n  will print out extra summary information of the days trades\n"
                << "  -v, --verbose\n  will print out extra summary info while trades are being executed\n"
                << "  -m, --median\n  will print the current median match price for each equity\n"
                << "  -t, --transfers\n  will print the the net amount of funds transfereed by all clients\n"
                << "  -i, --insider EQUIT_SYMBOL\n  allows the program to insert its own buys and"
                << "  sells at opportune moments during the day\n"
                << "  -g, --ttt EQUITY_SYMBOL \n  the program determines the best time in the day to buy\n"
                << "  and sell a particular stock.\n"
                << "  -h, --help\n  prints this help message.\n";
                exit(0);
                break;
            case '?':
                cout << "Unrecognized command line arguments" << endl;
                exit(1);
                break;
        }
    }
    ttt_orders.resize(ttt_equities.size());
    //-----------------------INPUT(START OF FILE)-------------------------
    string input_type, current;
    long long random_seed, amount_orders, ID = 0;
    char last_client, last_equity;
    double arrival_rate;
    long long timestamp = 0, price = 0, quantity = 0, generator_timestamp = 0;
    string client_name, buy_sell, equity_symbol, price_string, quantity_string;
    cin >> input_type;
    mt19937 gen;
    uniform_int_distribution<char> clients;
    uniform_int_distribution<char> equities;
    exponential_distribution<> arrivals;
    bernoulli_distribution BUY_SELL;
    uniform_int_distribution<> PRICE;
    uniform_int_distribution<> QUANTITY;
    if(input_type == "PR"){
        cin >> current >> random_seed;
        cin >> current >> amount_orders;
        cin >> current >> last_client;
        cin >> current >> last_equity;
        cin >> current >> arrival_rate;
        gen = mt19937(random_seed);
        clients = uniform_int_distribution<char>('a', last_client);
        equities = uniform_int_distribution<char>('A', last_equity);
        arrivals = exponential_distribution<>(arrival_rate);
        PRICE = uniform_int_distribution<>(2, 11);
        QUANTITY = uniform_int_distribution<>(1, 30);
    }
    cin >> timestamp;
    long long current_timestamp = 0;
    long long previous_timestamp = timestamp;
    long long transaction_total = 0;
    long long commission_total = 0, commission_current = 0;
    long long total_trades = 0;
    long long stocks_traded = 0;
    for(unsigned int i = 0; i < insider_equities.size(); ++i){
        string transfer_insider = "INSIDER_" + insider_equities[i];
        transfer_tracker[transfer_insider];
    }
    while(cin || amount_orders > 0){
        //ADD INSIDERS
        
        //ADD NEW TO THE FILE.
        if(input_type == "TL"){
            if(timestamp < 0 || previous_timestamp > timestamp){
                cout << "incorrect timestamp\n";
                exit(1);
            }
            cin >> client_name;
            if(client_name.size() == 0 || client_name == "BUY" || 
	       client_name == "SELL" || !check_string(client_name)){//check for alpha numeric
                cout << "incorrect client name\n";
		exit(1);
            }
            cin >> buy_sell;
            if(buy_sell != "BUY" && buy_sell != "SELL"){//check for buy, sell
                cout << "not buy or sell\n";
                exit(1);
            }
            cin >> equity_symbol;
            if(equity_symbol.size() < 1 || equity_symbol.size() > 5){//check for correct equity size
                cout << "equity symbol wrong size\n";
                exit(1);
            }
            cin >> price_string;
            if(price_string[0] != '$'){//check for correct $ placement
                cout << "incorrect $ placement\n";
                exit(1);
            }
            price = atoi((price_string.substr(1)).c_str());
            if(price <= 0){//check for non-negative number
                cout << "negative price\n";
                exit(1);
            }
            cin >> quantity_string;
            if(quantity_string[0] != '#'){//check for correct # placement.
                cout << "incorrect # placement\n";
                exit(1);
            }
            quantity = atoi((quantity_string.substr(1)).c_str());
        }
        else{//If pseudorandom input, generate the necessary values.
            timestamp = generator_timestamp;
            generator_timestamp = generator_timestamp + floor(arrivals(gen) + .5);
            client_name = string("C_") + clients(gen);
            buy_sell = (BUY_SELL(gen) ? "BUY" : "SELL");
            equity_symbol = string("E_") + equities(gen);
            price = 5 * PRICE(gen);
            quantity = QUANTITY(gen);
        }
        
        if(current_timestamp != timestamp){//update the median and print for all equities with real values.
            if(median_true){
                if(!median_tracker.empty()){
                    for(auto i = median_tracker.begin(); i != median_tracker.end(); ++i){
                        if((i->second).get_median() != 0){
                            out << "Median match price of " << i->first << " at time ";
                            out << current_timestamp << " is $" << (i->second).get_median();
                            out << "\n";
                            
                        }
                    }
                }
            }
            current_timestamp = timestamp;
        }
        
        if(buy_sell == "BUY"){//If the order is a buy order, than put it into the ttt book and the buy book structure.
            order* new_order = new order(true, ID, timestamp, price, quantity, client_name, equity_symbol);
            buy_book[equity_symbol].push(new_order);
            clean_up.push_back(new_order);
            if(ttt_equities.size() > 0){
                if(check_insider(ttt_equities, new_order->get_equity()) != ttt_equities.size() + 1){
                    ttt_orders[check_insider(ttt_equities, new_order->get_equity())].push_back(new_order);
                }
            }
        }
        else{//If the order is a sell order, then put it into the ttt book and the sell book structure.
            order* new_order = new order(false, ID, timestamp, price, quantity, client_name, equity_symbol);
            sell_book[equity_symbol].push(new_order);
            clean_up.push_back(new_order);
            if(ttt_equities.size() > 0){
                if(check_insider(ttt_equities, equity_symbol) != ttt_equities.size() + 1){
                    ttt_orders[check_insider(ttt_equities, new_order->get_equity())].push_back(new_order);
                }
            }
        }
        
        ++ID;
        if(transfer_true){//If transfers were specified, create a new transfer structure at the client_name.
            transfer_tracker[client_name];
        }
	//While the most expensive buy is more expensive than the least expensive sell, match trades.
        if(!sell_book[equity_symbol].empty() && !buy_book[equity_symbol].empty()){
            while(!sell_book[equity_symbol].empty() && !buy_book[equity_symbol].empty() &&
                  (sell_book[equity_symbol].top())->get_price() <= (buy_book[equity_symbol].top())->get_price()){
                order* sell_top = sell_book[equity_symbol].top();
                sell_book[equity_symbol].pop();
                order* buy_top = buy_book[equity_symbol].top();
                buy_book[equity_symbol].pop();
		//update the median if necessary.
                if(median_true || insider_equities.size() != 0){
                    if(sell_top->get_ID() > buy_top->get_ID()){
                        median_tracker[sell_top->get_equity()].push(buy_top);
                    }
                    else{
                        median_tracker[buy_top->get_equity()].push(sell_top);
                    }
                }
		//update the transfer if necessary
                if(transfer_true){
                    transfer_tracker[sell_top->get_client()].stock_sold(sell_top, buy_top);
                    transfer_tracker[buy_top->get_client()].stock_bought(sell_top, buy_top);
                }
                stocks_traded = stocks_traded + min(buy_top->get_quantity(), sell_top->get_quantity());
                commission_current = buy_top->buy_stock(sell_top, verbose, out);
                //generate the numbers for the summary and update.
		if(summary){
                    transaction_total = transaction_total + commission_current;
                    commission_current = commission_current / 100;
                    commission_current = 2 * commission_current;
                    commission_total = commission_current + commission_total;
                }
		//If not all the buy or sell order was fulfilled, than replace in the correct structure.
                if(buy_top->get_quantity() != 0){
                    buy_book[equity_symbol].push(buy_top);
                }
                if(sell_top->get_quantity() != 0){
                    sell_book[equity_symbol].push(sell_top);
                }
                ++total_trades;
            }
        }
	//While there are trades that the insider would trade on, greater than or less than 10% of median.
        string current_insider = "INSIDER_" + equity_symbol;
        if(check_insider(insider_equities, equity_symbol) != insider_equities.size() + 1 && 
	   median_tracker[equity_symbol].get_median() != 0){
            while(!sell_book[equity_symbol].empty() && sell_book[equity_symbol].top()->get_price() < 
		  median_tracker[equity_symbol].get_median() - median_tracker[equity_symbol].get_median() / 10){
	      //update the transfers for the insider.
	      transfer_tracker[current_insider].stock_bought_insider(sell_book[equity_symbol].top());
	      transfer_tracker[sell_book[equity_symbol].top()->get_client()].stock_to_insider(sell_book[equity_symbol].top());
		//update the 
                median_tracker[equity_symbol].push(sell_book[equity_symbol].top());
		//If verbose is specified, than print out the trade.
		if(verbose){
                    out << "INSIDER_" + equity_symbol << " purchased " << sell_book[equity_symbol].top()->get_quantity();
                    out << " shares of " << equity_symbol << " from " << sell_book[equity_symbol].top()->get_client();
                    out << " for $" << sell_book[equity_symbol].top()->get_price() << "/share\n";
                }
		//add the insider order to the ttt structure.
                if(ttt_equities.size() > 0){
                    if(check_insider(ttt_equities, equity_symbol) != ttt_equities.size() + 1){
                        order* insider = new order(true, ID, current_timestamp, sell_book[equity_symbol].top()->get_price(),
                                                   sell_book[equity_symbol].top()->get_quantity(), current_insider, equity_symbol);
                        ttt_orders[check_insider(ttt_equities, equity_symbol)].push_back(insider);
                        clean_up.push_back(insider);
                        ++ID;
                    }
                }
		//update the summary statistics
                if(summary){
                    commission_current = sell_book[equity_symbol].top()->get_quantity() * 
		      sell_book[equity_symbol].top()->get_price();
		    
		    transaction_total = transaction_total + commission_current;
                    commission_current = commission_current / 100;
                    commission_current = 2 * commission_current;
                    commission_total = commission_total + commission_current;
                    stocks_traded = stocks_traded + sell_book[equity_symbol].top()->get_quantity();
                    ++total_trades;
                }
                
                sell_book[equity_symbol].pop();
            }
        }
	//check if any insider trades can occur for buy orders.
        if(check_insider(insider_equities, equity_symbol) != insider_equities.size() + 1 && 
	   median_tracker[equity_symbol].get_median() != 0){
            while(!buy_book[equity_symbol].empty() && buy_book[equity_symbol].top()->get_price()
                  > median_tracker[equity_symbol].get_median() + median_tracker[equity_symbol].get_median() / 10){
              //update transfers  
	      transfer_tracker[current_insider].stock_sold_insider(buy_book[equity_symbol].top());
	      transfer_tracker[buy_book[equity_symbol].top()->get_client()].stock_from_insider(buy_book[equity_symbol].top());
	      //update median
	      median_tracker[equity_symbol].push(buy_book[equity_symbol].top());
	      //print the trade if necessary.
	      if(verbose){
		out << buy_book[equity_symbol].top()->get_client() << " purchased ";
		out << buy_book[equity_symbol].top()->get_quantity();
		out << " shares of " << equity_symbol << " from " << "INSIDER_" + equity_symbol;
		out << " for $" << buy_book[equity_symbol].top()->get_price() << "/share\n";
	      }
	      //add the insider order to the ttt queue.
	      if(ttt_equities.size() > 0){
		if(check_insider(ttt_equities, equity_symbol) != ttt_equities.size() + 1){
		  order* insider = new order(false, ID, current_timestamp, buy_book[equity_symbol].top()->get_price(),
					     buy_book[equity_symbol].top()->get_quantity(), current_insider, 
					     equity_symbol);
                        ttt_orders[check_insider(ttt_equities, equity_symbol)].push_back(insider);
                        clean_up.push_back(insider);
                        ++ID;
                    }
                }
	      //update the summary statistics
	      if(summary){
		commission_current = buy_book[equity_symbol].top()->get_quantity() * 
		  buy_book[equity_symbol].top()->get_price();
		transaction_total = transaction_total + commission_current;
		commission_current = commission_current / 100;
		commission_current = 2 * commission_current;
		commission_total = commission_current + commission_total;
		stocks_traded = stocks_traded + buy_book[equity_symbol].top()->get_quantity();
		++total_trades;
	      }
	      buy_book[equity_symbol].pop();
            }
        }
        --amount_orders;
        previous_timestamp = timestamp;
        cin >> timestamp;
        
    }//while
    //print out the median.
    if(median_true && !median_tracker.empty()){
        for(auto i = median_tracker.begin(); i != median_tracker.end(); ++i){
	  if((i->second).get_median() != 0){
            out << "Median match price of " << i->first << " at time " << current_timestamp;
            out << " is $" << (i->second).get_median() << "\n";
	  }
        }
    }
    //now the end of the day.
    out << "---End of Day---\n";
    if(summary){//print summary statistics
        out << "Commission Earnings: $" << commission_total << "\n";
        out << "Total Amount of Money Transferred: $" << transaction_total << "\n";
        out << "Number of Completed Trades: " << total_trades << "\n";
        out << "Number of Shares Traded: " << stocks_traded << "\n";
    }
    //print the transfers
    if(transfer_true){
        for(auto i = transfer_tracker.begin(); i != transfer_tracker.end(); ++i){
            out << i->first << " bought " << (i->second).get_number_bought() << " and sold ";
            out << (i->second).get_number_sold() << " for a net transfer of $";
            out << (i->second).get_net_transfer() << "\n";
        }
    }
    //print the time travelers
    if(!ttt_equities.empty()){
        for(unsigned int i = 0; i < ttt_equities.size(); ++i){
            pair<vector<order*>::const_iterator, vector<order*>::const_iterator> current_pair = time_traveler(ttt_orders[i]);
            if(current_pair.first != ttt_orders[i].end() && current_pair.second != ttt_orders[i].end()){
	      out << "Time travelers would buy " << ttt_equities[i] << " at time: ";
	      out << (*(current_pair.first))->get_timestamp();
	      out << " and sell it at time: " << (*(current_pair.second))->get_timestamp() << "\n";
            }
            else{
	      out << "Time travelers would buy " << ttt_equities[i] << " at time: -1 and sell it at time: -1\n";
            }
        }
    }
    cout << out.str();
    out.flush();
    //clean up all memory
    for(unsigned int i = 0; i < clean_up.size(); ++i){
      delete clean_up[i];
    }
    
    return 0;
}
