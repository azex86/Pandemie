#include "Entity.hpp"


Simulation* Entity::simulation = nullptr;

void Entity::update()
{

}

void Entity::updateSimulationSettings()
{
}

const Vecteur& Entity::getPos() const
{
    return pos;
}

const Vecteur& Entity::getSize() const
{
    return size;
}

void Entity::setPos(const Vecteur& _pos)
{
    vector<Chunk*> formerChunks = simulation->getChunks(this);
    pos = _pos;
    vector<Chunk*> newChunks = simulation->getChunks(this);
    for(Chunk* c : formerChunks)
        if(find(newChunks.begin(),newChunks.end(),c)==newChunks.end())
        {
            c->removeEntity(this);
        }else
        {
            newChunks.erase(find(newChunks.begin(),newChunks.end(),c));
        }
    for(Chunk* c : newChunks)
        c->addEntity(this);
}

void Entity::move(const Vecteur& m)
{
    this->setPos(pos+m);
}

void Entity::setSize(const Vecteur& _size)
{
    vector<Chunk*> formerChunks = simulation->getChunks(this);
	size = _size;
    vector<Chunk*> newChunks = simulation->getChunks(this);
    for (Chunk* c : formerChunks)
        if (find(newChunks.begin(), newChunks.end(), c) == newChunks.end())
        {
            c->removeEntity(this);
        }
        else
        {
            newChunks.erase(find(newChunks.begin(), newChunks.end(), c));
        }
    for (Chunk* c : newChunks)
        c->addEntity(this);
}


sf::Shape* Entity::updateSprite()
{
    return sprite;
}

Entity::~Entity()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

//Default collision between one entity and one chunk : rect collision
bool Entity::collision(const Entity* e)const
{
    return this->collision(e->pos,e->size);
}

Entity::Entity(const Vecteur& pos, const Vecteur& size) :pos(pos), size(size), sprite(nullptr)
{
}

//Default collision between one entity and one chunk : rect collision
bool Entity::collision(const Chunk* chunk)const
{
    return collision(chunk->getPos(),chunk->getSize());
}

//collsion between two rect
bool Entity::collision(const Vecteur& _pos,const Vecteur& _size)const
{
    
    bool collision_x = !(((pos.x + size.x) < _pos.x) || (pos.x > (_pos.x + _size.x)));
    bool collision_y = !(((pos.y + size.y) < _pos.y) || (pos.y > (_pos.y + _size.y))); 
    return collision_x && collision_y;
}

