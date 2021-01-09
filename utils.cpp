#include "utils.hpp"


std::vector<std::string> split(std::string phrase , char separator)
{
    std::vector<std::string> res;
    unsigned int debut = 0;
    unsigned int nbchar = 0;
    for(unsigned int i = 0; i< phrase.size(); i++)
    {
        if(phrase[i] == separator)
        {
            res.push_back(phrase.substr(debut,nbchar));
            nbchar = 0;
            debut = i+1;
        }
        else
        {
            nbchar++;
        }
    }
    if(debut < phrase.size())
    {
        res.push_back(phrase.substr(debut,nbchar));
    }
    
    return res;
}


