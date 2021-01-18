#include "joueur.hpp"


Role::Role(std::string name): m_name(name)
{
    if(name == "loup_garou")
    {
        camp = 1;
    }
    else
    {
        camp = 0;
    }

}

Joueur::Joueur(std::string pseudo): m_role("none"), awake(true), vie(true), pseudo(pseudo), immunity(false)
{

}

void Joueur::reveiller()
{
    awake = true;
}

std::string Joueur::get_pseudo() const 
{
    return pseudo;
}

void Joueur::set_role(std::string newRole)
{
    m_role = newRole;
}

std::string Joueur::get_role() const
{
    return m_role;
}

