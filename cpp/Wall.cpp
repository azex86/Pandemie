#include "Wall.hpp"

Wall::Wall(const Vecteur& pos, const Vecteur& size, const sf::Color& _color) :Entity(pos, size), enabled(true), color(_color)
{
	sprite = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
	sprite->setFillColor(color);
	sprite->setPosition(pos.x, pos.y);
}

bool Wall::collision(const Chunk* chunk) const
{
	if (!enabled)
		return false;
	else
	{
		return Entity::collision(chunk);
	}
}

bool Wall::collision(const Vecteur& _pos, const Vecteur& _size) const
{
	if (!enabled)
		return false;
	else
	{
		return Entity::collision(_pos, _size);
	}
}

bool Wall::collision(const Entity* e) const
{
	if (!enabled)
		return false;
	else
	{
		return Entity::collision(e);
	}
}

void Wall::enable()
{
	enabled = true;
}

void Wall::disable()
{
	enabled = false;
}

bool Wall::isEnabled()
{
	return enabled;
}

sf::Shape* Wall::updateSprite()
{
	if (enabled)
		sprite->setFillColor(color);
	else
		sprite->setFillColor(sf::Color::Transparent);
	
	sprite->setPosition(pos.x, pos.y);

	return Entity::updateSprite();
}
