#pragma once
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include "Vecteur.hpp"
#include "Entity.hpp"
using namespace std;

class Entity;
class Simulation;

class Chunk
{
    friend class Simulation;
    list<Entity*> entities;
    Vecteur pos;
    Vecteur size;

public:
    Chunk();
    Chunk(const Vecteur& pos,const Vecteur& size);
    void addEntity(Entity* e);
    void removeEntity(Entity* e);
    bool collision(const Vecteur& pos,const Vecteur& size);
    bool collision(const Vecteur& pos);
    const Vecteur& getPos()const;
    const Vecteur& getSize()const;
	//return all the entities in the chunk
	const list<Entity*>& getEntities()const;
    sf::RectangleShape drawPerimeter()const;
};