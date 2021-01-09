#include "Personnage.hpp"


Personnage::Personnage(std::string name, int dcamp): intitul(name), vie(true), maire(false), camp(dcamp), amour(""), phaseActive("none")
{
}

Personnage::~Personnage()
{
}

std::string Personnage::get_phase() const
{
	return phaseActive;
}

std::string Personnage::get_intitul() const 
{
	return intitul;
}

Sorciere::Sorciere(): Personnage("sorciere", 0), potionVie(true), potionMort(true)
{
}

Villageoi::Villageoi(): Personnage("villageoi", 0)
{
	
}

Voyante::Voyante(): Personnage("voyante", 0)
{
}

LoupGarou::LoupGarou(): Personnage("loup_garou", 1)
{
}

Cupidon::Cupidon(): Personnage("cupidon", 0)
{
}

PetiteFille::PetiteFille(): Personnage("petite_fille", 0)
{
}

Salvateur::Salvateur() : Personnage("salvateur", 0), lastJoueur("")
{
}

Chasseur::Chasseur() : Personnage("chasseur",0)
{
}
