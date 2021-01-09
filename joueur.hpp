#ifndef JOUEUR_HPP
#define JOUEUR_HPP
#include "serveur.hpp"
/// camp : 0 village, 1 loup_garou, 2 couple , -1 seul 

class  Role
{
private:
    std::string m_name;

public: 
    Role(std::string name);
    int camp;



};
// pas besoin de de faire des roles specifique coté serveur parceque on affiche rien 


class Joueur
{
private:
    std::string m_role;
    bool awake;
    bool vie;
    std::string pseudo;
    bool immunity; // pas forcement utile (pour test tu enleve et tu recompile)

public:
    Joueur(std::string name);
    void reveiller();
    void endormir();
    std::string get_pseudo();
    std::string get_role() const;
    void set_role(std::string newRole);
};





#endif