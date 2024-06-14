#include "Lutin.hpp"

//return if there is any object with rect collision with lutin
bool Lutin::collision()
{
	for (Chunk* c : simulation->getChunks(this))
		for (Entity* e : c->getEntities())
			if (e != this && e->collision(this))
				return true;
	return false;
}

Lutin::Lutin(const Vecteur& pos, const Vecteur& size, const sf::Color& _color) :Entity(pos, size),blocked(false)
{
	sprite = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
	sprite->setFillColor(_color);
	sprite->setPosition(pos.x, pos.y);
}

sf::Shape* Lutin::updateSprite()
{
	sprite->setPosition(pos.x, pos.y);
	if(blocked)
		sprite->setFillColor(sf::Color::Red);
	else
		sprite->setFillColor(sf::Color::Blue);
	return Entity::updateSprite();
}

void Lutin::update()
{
	auto& settings = *simulation->getSettings();
	pos = Vecteur(stoi(settings["x"]), stoi(settings["y"]));

	//get key event
	Vecteur m(0, 0);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m.y -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m.y += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m.x += 1;


	if (m == Vecteur(0, 0))
		return;
	//move lutin
	move(m);
	//check collision
	if (collision())
	{
		move(-m);
		blocked = true;
	}
	else if (pos.x<0 || pos.x+size.x > simulation->getSize().x || pos.y<0 || pos.y+size.y>simulation->getSize().y)
	{
		move(-m);
		blocked = true;
	}
	else
	{
		sprite->setFillColor(sf::Color::Blue);
		blocked = false;
	}

	settings["x"] = to_string(pos.x);
	settings["y"] = to_string(pos.y);
}
