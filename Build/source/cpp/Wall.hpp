#pragma once
#include "Entity.hpp"

#ifndef _WALL_HEADER



class Entity;

class Wall :
    public Entity
{
	bool enabled;
	sf::Color color;
public:
	Wall(const Vecteur& pos, const Vecteur& size,const sf::Color& _color = sf::Color::White);
	bool collision(const Chunk* chunk)const;
	bool collision(const Vecteur& _pos, const Vecteur& _size)const;
	bool collision(const Entity* e)const;
	void enable();
	void disable();
	bool isEnabled();
	sf::Shape* updateSprite();
};

#define _WALL_HEADER
#endif // !_WALL_HEADER
