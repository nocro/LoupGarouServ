#ifndef UTILS_HPP
#define UTILS_HPP

#include <string> 
#include <vector> 
#include <iostream>

std::vector<std::string> split(std::string phrase, char separator = ' ');


// les template sont a definir dans le .hpp 
template<typename T> 
void print(std::vector<T> tab, std::string separator = " ")
{
    for(T e : tab)
    {
        std::cout << e << separator;
    }
    std::cout << std::endl;
}

















#endif