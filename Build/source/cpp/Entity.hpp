#pragma once   
#define NOMINMAX
#include "Vecteur.hpp"
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace std;

class Simulation;
class Chunk;

class Entity
{
protected:
    friend class Simulation;
    static Simulation* simulation;

    Vecteur pos;
    Vecteur size;

	sf::Shape* sprite;

public:
	Entity(const Vecteur& pos, const Vecteur& size);
    virtual bool collision(const Chunk* chunk)const;
    virtual bool collision(const Vecteur& _pos,const Vecteur& _size)const;
    virtual bool collision(const Entity* e)const;

	//iterates the state of the entity
	//note, simulation->settings is get with a lock before update call
    virtual void update();

    virtual void updateSimulationSettings();

	const Vecteur& getPos()const;
	const Vecteur& getSize()const;
    void setPos(const Vecteur& _pos);
    void move(const Vecteur& m);
    void setSize(const Vecteur& _size);



	virtual sf::Shape* updateSprite();

    ~Entity();
};

#include "Chunk.hpp"
#include "Simulation.hpp"