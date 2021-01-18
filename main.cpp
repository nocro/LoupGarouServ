#include "partie.hpp"
#include "utils.hpp"

int main()
{
    // Server serv;
    // serv.run();
    // std::cout << "fin :) " << std::endl;

    do
    {
        Partie p;
        p.demarage();
        p.run();
        std::cout << "fin :)\nContinuer ? y/n " << std::endl;
        std::string s;
        std::cin >> s;
        if(s[0]=='n')
        {
            break;
        }
        
    }while(true);
    
    


    return 0;
}