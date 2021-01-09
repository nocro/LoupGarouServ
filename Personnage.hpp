#pragma once

#include <string>

/// camp : 0 village, 1 loup_garou, 2 couple, -1 seul

class Personnage
{
private:
	//int camp;//utile ?
	std::string intitul;
	bool vie;
	bool maire;
	int camp;
	std::string amour; // si amour != "" <=> camp = 2 
	std::string phaseActive;

public:
	Personnage(std::string name, int camp);
	~Personnage();

public:
	std::string get_intitul() const ;
	std::string get_phase() const ;
};

class Villageoi : public Personnage
{
public:
	Villageoi();

};

class Voyante: public Personnage
{
public:
	Voyante();

};

class LoupGarou : public Personnage
{
public:
	LoupGarou();

};

class Sorciere : public Personnage
{
private:
	bool potionVie;
	bool potionMort;
public:
	Sorciere();

};

class Cupidon : public Personnage
{
public:
	Cupidon();

};

class PetiteFille : public Personnage
{
public:
	PetiteFille();

};

class Salvateur : public Personnage
{
private :
	std::string lastJoueur;
public :
	Salvateur();

};

class Chasseur : public Personnage
{
public :
	Chasseur();

};

