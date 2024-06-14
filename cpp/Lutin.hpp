#pragma once
#include "Entity.hpp"

class Lutin : public Entity
{
private:
	bool blocked;
	bool collision();
public:
	Lutin(const Vecteur& pos, const Vecteur& size,const sf::Color& _color);
	virtual sf::Shape* updateSprite() override;
	virtual void update() override;

};