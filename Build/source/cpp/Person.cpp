#include "Person.hpp"


double Person::infection_range = 5;
double Person::infection_range_square = 25;
ushort Person::infection_time = 10;
ushort Person::immunity_time = 10;
double Person::radius_sprite = 2;
double Person::infection_rate = 0.8;
double Person::confinement = 0;
double Person::old_confinment = 0;
double Person::quarantine = 0;
ushort Person::quarantine_time = 5;
bool Person::tryInfect(Person* pathogen)
{
	//if the pathogen is infected and the person is not infected
	if (rand() / (double)RAND_MAX < infection_rate)
	{
		setState(State::Infected);

		
		return true;
	}
	return false;
}

void Person::incrementCounter()
{
	if (state == State::Infected)
	{
		time_infected++;
		if (time_quarantined != 0)
			time_quarantined++;
		if (tryDead())
			return;
		tryRecover();
		
	}
	else if (state == State::Recovered)
	{
		time_recovered++;
		tryImmune();
	}

	tryQuarantine();
	if (time_confined = 0)
		time_confined++;
}

void Person::setState(State s)
{
	size_t* counter = simulation->getCounter();
	if (s == Person::Infected)
		int a = 5;
	counter[state]--;
	counter[s]++;
	state = s;
	time_recovered = 0;
	time_infected = 0;
}
bool Person::tryRecover()
{
	if (time_infected > infection_time)
	{
		setState(State::Recovered);
		if(time_quarantined!=0 && quarantine!=0)
			leaveQuarantine();
		return true;
	}
	else
		return false;
}
bool Person::tryDead()
{
	return false;
}
bool Person::tryImmune()
{
	if (time_recovered > immunity_time)
	{
		setState(State::Normal);
		return true;
	}
	else
		return false;
}
void Person::tryQuarantine()
{
	//if the pathogen is quarantined, the person is quarantined
	if (Person::state == State::Infected)
		time_quarantined++;
	if (time_quarantined == quarantine_time && speed != Vecteur(0,0))
	if (rand() / (double)RAND_MAX < quarantine)
	{
		temp_speed = speed;
		speed = Vecteur(0, 0);
	}
}
void Person::tryConfinment()
{
	//the person is confined
	if (rand() / (double)RAND_MAX < confinement)
	{
		temp_speed = speed;
		speed = Vecteur(0, 0);
		time_confined = 1;
	}
	else if (time_confined != 0)
	{
		speed = temp_speed;
		time_confined = 0;
	}
}
void Person::leaveQuarantine()
{
	speed = temp_speed;
	time_quarantined = 0;
}

Person::Person(const Vecteur& pos, const Vecteur& _speed, State _state) 
	:Entity(pos, Vecteur(infection_range, infection_range)), speed(_speed), state(_state),
	time_infected(0), time_recovered(0), time_quarantined(0), time_confined(0)
{
	simulation->getCounter()[_state]++;
	sprite = new sf::CircleShape(radius_sprite);
}

sf::Shape* Person::updateSprite()
{
	sprite->setPosition(pos);
	switch (this->state)
	{
	case State::Normal:
		sprite->setFillColor(sf::Color::White);
		break;
	case State::Recovered:
		sprite->setFillColor(sf::Color::Blue);
		break;
	case State::Infected:
		sprite->setFillColor(sf::Color::Red);
		break;
	case State::Dead:
		sprite->setFillColor(sf::Color::Transparent);
		break;
	default:
		break;
	}
	return Entity::updateSprite();
}

void Person::update()
{
	bool collision_x = false;
	bool collision_y = false;
	move(speed);
	if (pos.x > simulation->getSize().x || pos.x<0)
		collision_x = true;
	if (pos.y > simulation->getSize().y || pos.y<0)
		collision_y = true;

	this->incrementCounter();
	//get all entities in the same chunk
	for (Chunk* c : simulation->getChunks(this))
			for (Entity* e : c->getEntities())
			{
				if (e != this && e->collision(this))
				{
					if (state == State::Normal)
					{
						//if the entity is a person and is not infected
						Person* p = dynamic_cast<Person*>(e);
						if (p && p->state == State::Infected)
						{
							//try to infect the entity
							this->tryInfect(p);
						}
					}
					Wall* wall = dynamic_cast<Wall*>(e);
					if (wall && wall->isEnabled())
					{
						collision_x = collision_y = true;
					}
				}
			}

	if(collision_x)
		speed.x = -speed.x;
	if(collision_y)
		speed.y = -speed.y;
}

void Person::updateSimulationSettings()
{
	setSize(Vecteur(infection_range, infection_range));
	if (old_confinment != confinement)
		tryConfinment();
}

bool Person::collision(const Entity* e) const
{
	Vecteur temp_pos = (this->pos - (*e).getPos());
	double magnitude = temp_pos.magnitude();

	if (magnitude < infection_range_square)
		return true;
	else
		return false;
}

void Person::setInfectionRange(double range)
{
	Person::infection_range = range;
	Person::infection_range_square = range * range;
}

void Person::setInfectionTime(double time)
{
	Person::infection_time = time;
}

double Person::getInfectionRange()
{
	return Person::infection_range;
}

double Person::getInfectionTime()
{
	return Person::infection_time;
}

void Person::setInfectionRate(double rate)
{
	Person::infection_rate = rate;

}


double Person::getInfectionRate()
{
	return Person::infection_rate;
}

void Person::setImmunityTime(double time)
{
	Person::immunity_time = time;

}

double Person::getImmunityTime()
{
	return immunity_time;
}

void Person::setConfinement(double rate)
{
	Person::old_confinment = Person::confinement;
	Person::confinement = rate;
}

double Person::getConfinement()
{
	return Person::confinement;
}

void Person::setQuarantine(double rate)
{
	Person::quarantine = rate;
}

double Person::getQuarantine()
{
	return Person::quarantine;
}

void Person::setQuarantineTime(double time)
{
	Person::quarantine_time = time;
}

double Person::getQuarantineTime()
{
	return Person::quarantine_time;
}
