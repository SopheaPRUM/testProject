#include <string>
#include <algorithm>
#include <iterator>
#include "Tools.hpp"

std::vector<string> Tools::stringToStringVector(const std::string& str, char delim)
{
	std::vector<string> cont;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
    return cont;
}