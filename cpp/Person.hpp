#pragma once
#include "Entity.hpp"
#include "Wall.hpp"

#ifndef _PERSON_HEADER

class Entity;
typedef unsigned short ushort;

class Person : public Entity
{
public:
	enum  State
	{
		Normal,
		Recovered,
		Infected,
		Dead,
		END
	};
private:
	State state;
	static double infection_range; //range of infection
	static double infection_range_square; //range of infection squared
	static double infection_rate;//Probability to infect
	static ushort infection_time; //Time to recover after infection
	static ushort immunity_time; //Time to lost immunity after recovery
	static double confinement;//Probability to be confined equal to not move
	static double old_confinment;
	static ushort quarantine_time;//Time before to be quarantined
	static double quarantine;//Probability to be quarantined when infected

	ushort time_infected;//Time since infection
	ushort time_recovered;//Time since recovery
	ushort time_confined;//Time since confinement
	ushort time_quarantined;//Time since quarantine
	Vecteur temp_speed;//Speed before quarantine or confinment
	//Try to infect this, in accord of this state and the other state
	//pathogen is the person who try to infect 
	bool tryInfect(Person* pathogen);
	void incrementCounter();
	void setState(State s);;
	bool tryRecover();
	bool tryDead();
	bool tryImmune();
	void tryQuarantine();
	void leaveQuarantine();
	void tryConfinment();

	static double radius_sprite;

	Vecteur speed;

public:
	Person(const Vecteur& pos, const Vecteur& _speed,State _state = Person::State::Normal);

	virtual sf::Shape* updateSprite() override;

	virtual void update() override;
	virtual void updateSimulationSettings() override;
	virtual bool collision(const Entity* e)const override;

	static void setInfectionRange(double range);
	static void setInfectionTime(double time);
	static double getInfectionRange();
	static double getInfectionTime();
	static void setInfectionRate(double rate);
	static double getInfectionRate();
	static void setImmunityTime(double time);
	static double getImmunityTime();
	static void setConfinement(double rate);
	static double getConfinement();
	static void setQuarantine(double rate);
	static double getQuarantine();
	static void setQuarantineTime(double time);
	static double getQuarantineTime();
};

#define _PERSON_HEADER
#endif // !_PERSON_HEADER