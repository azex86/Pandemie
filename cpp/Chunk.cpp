#include "Chunk.hpp"

Chunk::Chunk():pos(),size()
{

}

Chunk::Chunk(const Vecteur& _pos,const Vecteur& _size):pos(_pos),size(_size)
{

}

const Vecteur& Chunk::getPos()const
{
    return this->pos;
}

const Vecteur& Chunk::getSize()const
{
    return this->size;
}

const list<Entity*>& Chunk::getEntities() const
{
	return entities;
}

sf::RectangleShape Chunk::drawPerimeter() const
{
    using namespace sf;
	
    sf::RectangleShape shape(size);
    shape.setPosition(pos);
	shape.setOutlineColor((this->entities.size()==0)?sf::Color::Yellow:sf::Color::Cyan);
	shape.setOutlineThickness(-2);
    shape.setFillColor(Color::Transparent);
	return shape;

}

void Chunk::addEntity(Entity* e)
{
    entities.push_back(e);
}

void Chunk::removeEntity(Entity* e)
{
    entities.remove(e);
}

bool Chunk::collision(const Vecteur& pos)
{
    return pos.x >= this->pos.x && pos.x <= this->pos.x + this->size.x && pos.y >= this->pos.y && pos.y <= this->pos.y + this->size.y;
}

bool Chunk::collision(const Vecteur& _pos,const Vecteur& _size)
{
    bool collision_x = ((pos.x + size.x) < _pos.x) || (pos.x > (_pos.x + _size.x));
    bool collision_y = ((pos.y + size.y) < _pos.x) || (pos.y > (_pos.y + _size.y)); 
    return collision_x && collision_y;
}