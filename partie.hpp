#ifndef PARTIE_HPP
#define PARTIE_HPP

#include "joueur.hpp"
#include <vector> 
#include "serveur.hpp"
#include <algorithm>
#include "utils.hpp"

class Partie
{
private:
    std::vector<Joueur> joueurs; // on peut mettre le rôle dedans faudra juste lire tous les clients au lieux de lire juste les rôle actifs mais c'est negligeable
    std::vector<std::string> role_actif;// je sais pas si ya vraiment besoin de faire un type Role 
    std::map<std::string, int> role_joueur; // une map qui associe un role a l'indice du joueur concerné 
    std::map<std::string, std::string> pseudo_role; // associe un pseudo a un role

    Server server;
    bool enAttenteDeCo;
    std::thread portier; // celui qui vas gérer les connexions 

public:
    Partie();
    ~Partie();
    void demarage();
    void waitingClient();
    void run();
    bool isEnd();
    
};



#endif