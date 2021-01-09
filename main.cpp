#include "partie.hpp"
#include "utils.hpp"

int main()
{
    // Server serv;
    // serv.run();
    // std::cout << "fin :) " << std::endl;

    Partie p;
    p.demarage();
    p.run();
    std::cout << "fin :) " << std::endl;
    


    return 0;
}