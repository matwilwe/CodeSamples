#ifndef MARKET_HELPER_H
#define MARKET_HELPER_H
#include <cctype>
#include <string>
#include <utility>
#include "order.h"
#include <vector>

//Effects: returns true if the string contains only alphanumeric characters or underscores
bool check_string(const std::string &check);

//Effects: returns the index in the vector that contains the string current_client.
unsigned int check_insider(const std::vector<std::string> &insiders, const std::string &current_client);

//Effects: returns a pair of const iterators with the greatest difference in buy and sell price,
//given a vector of orders.
std::pair<std::vector<order*>::const_iterator, std::vector<order*>::const_iterator> time_traveler(const std::vector<order*> &ttt_arr);

#endif



