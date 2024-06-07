#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include "../../code/we/type/value.hpp"
#include "../../code/we/type/bitsetofint.hpp"
#include "../../code/boost/include/boost/variant.hpp"

using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>;

int main()
{
    // Sample data
    std::vector<std::tuple<int, std::vector<int>>> f = {
        {1, {10, 20, 30}},
        {2, {40, 50, 60}},
        {3, {70, 80, 90}} };

    pnet_list2d vec;

    // Loop through f and push elements into vec
    for (const auto& tuple : f)
    {
        int first = std::get<0>(tuple);
        const auto& second = std::get<1>(tuple);
        pnet_list sublist;

        sublist.push_front(pnet_value(first)); // Insert factor at the beginning
        for (const auto& value : second)
        {

            sublist.push_back(pnet_value(value));
        }
        vec.push_back(sublist);
    }

    for (pnet_list a : vec) {
        int factor = boost::get<int>(*a.begin()); // Get the factor
        std::vector<int> av;
        std::cout << " av is " << " ";
        std::cout << factor << "   ";

        for (auto it = std::next(a.begin()); it != a.end(); ++it)
        {
            auto intValue = boost::get<int>(*it);
            av.push_back(intValue);
            std::cout << intValue << " ";

        }
        std::cout << std::endl;
    }



    return 0;
}