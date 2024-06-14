#include "Simulation.hpp"



void Simulation::addEntity(Entity* e)
{
	univers.push_back(e);
	sprites.push_back(e->updateSprite());
	vector<Chunk*> chunks = getChunks(e);
	if (chunks.size() == 0)
	{
		cerr << "Entity out of bounds" << endl;
		delete e;
		return;
	}
    for(Chunk* chunk : chunks)
	    chunk->addEntity(e);
}

void Simulation::removeEntity(Entity* e)
{
	vector<Chunk*> chunks = getChunks(e);
    for(Chunk* chunk : chunks)
	    chunk->removeEntity(e);
	sf::Shape* temp = e->updateSprite();
	for (auto it = sprites.begin(); it != sprites.end(); it++)
	{
		if (*it == temp)
		{
			sprites.erase(it);
			break;
		}
	}
	for (auto it = univers.begin(); it != univers.end(); it++)
	{
		if (*it == e)
		{
			univers.erase(it);
			delete e;
			break;
		}
	}
}

void Simulation::clearEntities()
{
    for(Chunk* chunk : chunks)
        delete chunk;
	this->chunks.clear();
	sprites.clear();
	for (Entity* pe : this->univers)
		delete pe;
	this->univers.clear();
}

vector<Chunk*> Simulation::getChunks(const Vecteur& pos)
{
    vector<Chunk*> retour;
	for (Chunk* chunk : this->chunks)
	{
		if (chunk->collision(pos))
		{
			retour.push_back(chunk);
            //a point is in only one chunk
            //thus
            break;
		}
	}
    return retour;
}

vector<Chunk*> Simulation::getChunks(const Vecteur& pos, const Vecteur& size)
{
    vector<Chunk*> retour;
	for (Chunk* chunk : this->chunks)
	{
		if(chunk->collision(pos,size))
        {
            retour.push_back(chunk);
        }
	}
	return retour;
}

vector<Chunk*> Simulation::getChunks(const Entity& e)
{
	vector<Chunk*> retour;
	for (Chunk* chunk : this->chunks)
	{
		if(e.collision(chunk))
        {
            retour.push_back(chunk);
        }
	}
	return retour;
}

vector<Chunk*> Simulation::getChunks(const Entity* e)
{
    vector<Chunk*> retour;
	for (Chunk* chunk : this->chunks)
	{
		if(e->collision(chunk))
        {
            retour.push_back(chunk);
        }
	}
	return retour;
}

const vector<Chunk*>& Simulation::getChunks() const
{
	return chunks;
}

bool Simulation::getWallCollision(const Vecteur& pos, const Vecteur& size)
{
	for (Chunk* chunk : getChunks(pos, size))
	{
		for (Entity* e : chunk->getEntities())
		{
			Wall* wall = dynamic_cast<Wall*>(e);
			if (wall && wall->isEnabled() && wall->collision(pos, size))
				return true;
		}
	}
	return false;
}

bool Simulation::getWallCollision(const Entity* _e)
{
	for (Chunk* chunk : getChunks(_e))
	{
		for (Entity* e : chunk->getEntities())
		{
			if (_e != e)
			{
				Wall* wall = dynamic_cast<Wall*>(e);
				if (wall && wall->isEnabled() && wall->collision(_e))
					return true;
			}

		}
	}
	return false;
}

vector<sf::Shape*> Simulation::getSprites()
{
	if (sprites_updated)
	{
		return sprites;
	}
	else
	{
		settings_mutex.lock();
		for (Entity* e : univers)
			e->updateSprite();
		sprites_updated = true;
		settings_mutex.unlock();
		return sprites;
	}

}

const Vecteur& Simulation::getSize() const
{
	return size;
}

size_t Simulation::getIterations() const
{
	return iterations;
}

size_t Simulation::sumCounter()const
{
	size_t sum = 0;
	for (unsigned char i = 0; i < Person::END - 1; i++)
		sum += counter[i];
	return sum;
}

void Simulation::initCounter()
{
	for (unsigned char i = 0; i < Person::END - 1; i++)
		counter[i] = 0;
}

size_t Simulation::getCounter(unsigned short s) const
{
	return counter[s];
}

size_t* Simulation::getCounter() 
{
	return counter;
}

size_t Simulation::getSumCounter() const
{
	return sumCounter();
}

void Simulation::setSaveData(string filename)
{
	save_data = filename;
	if (save_file != nullptr)
	{
		save_file->clear();
		delete save_file;
	}
	save_file = new stringstream();
}

void Simulation::setPushData(bool push)
{
	push_data = push;
}

void Simulation::updateEntityChunks(Entity* e)
{
	for(Chunk* chunk : chunks)
	{
		chunk->removeEntity(e);
		if(e->collision(chunk))
			chunk->addEntity(e);
			
	}
}

Simulation::Simulation()
{
	running = false;
	paused = false;
	iterations = 0;
	n_chunk = 10;
	size = { 100,100 };
	Entity::simulation = this;
	simulation_thread = nullptr;
	settings = new std::unordered_map<std::string, std::string>();
	own_settings = true;
	initCounter();
	createChunks();
	createEntities();
	updateSettings();
}

Simulation::Simulation(std::unordered_map<std::string, std::string>& settings, const Vecteur& _size, size_t _n_chunk, size_t _n_thread)
	:size(_size), n_chunk(_n_chunk), n_thread(_n_thread)

{
	running = false;
	paused = false;
	iterations = 0;
	Entity::simulation = this;
	simulation_thread = nullptr;
	this->settings = &settings;
	own_settings = false;
	initCounter();
	createChunks();
	createEntities();
	updateSettings();
}

void Simulation::setSettings(std::unordered_map<std::string, std::string>& _settings)
{
	settings_mutex.lock();
	if(own_settings)
		delete settings;
	settings = &_settings;
	own_settings = false;
	updateSettings();
	settings_mutex.unlock();
}

std::unordered_map<std::string, std::string>* Simulation::getSettings(void)
{
	return this->settings;
}

mutex& Simulation::getSettingsMutex(void)
{
	return this->settings_mutex;
}

void Simulation::updateSettings(void)
{
	settings_mutex.lock();
	auto& rsettings = *settings;

	//update core settings
	wait_time = stoi(rsettings["wait_time"]);
	switch (stoi(rsettings["live"]))
	{
	case 0:
		running = false;
		break;
	case 1:
		running = true;
		paused = true;
		break;
	case 2:
		running = true;
		paused = false;
		break;
	default:
		break;
	}

	//update infection rules from "rules" in settings
	Person::setInfectionRange(stod(rsettings["rules[infection_range]"]));
	Person::setInfectionTime(stoi(rsettings["rules[infection_time]"]));
	Person::setInfectionRate(stod(rsettings["rules[infection_rate]"]));
	Person::setImmunityTime(stoi(rsettings["rules[immunity_time]"]));
	Person::setQuarantine(stod(rsettings["rules[quarantine]"]));
	Person::setConfinement(stod(rsettings["rules[confinement]"]));
	Person::setQuarantineTime(stoi(rsettings["rules[quarantine_time]"]));
	checkWallsSettings();

	//update counter from "counter" in settings
	while(stoi(rsettings["counter[All]"]) != sumCounter() && 0)
	{
		/*
		* 1 : il faut ajouter des personnes sans plus d'information (State::Normal) seul counter[All] a été modifié
		* 2 : les counters counter[Normal] et counter[Infected] et counter[Recovered] on été incrémenté
		* 3 : modification des rapports de counter[Normal] et counter[Infected] et counter[Recovered] et counter[Died] il faut changer l'état de personnes
		* 4 : il faut diminuer le nombre de personne
		*/

	}

	for (Entity* e : univers)
		e->updateSimulationSettings();

	settings_mutex.unlock();
}

void Simulation::pushData()
{
	settings_mutex.lock();
	auto& rsettings = *settings;
	rsettings["iterations"] =  to_string(iterations);
	rsettings["counter[Normal]"] = to_string(counter[Person::Normal]);
	rsettings["counter[Recovered]"] = to_string(counter[Person::Recovered]);
	rsettings["counter[Infected]"] = to_string(counter[Person::Infected]);
	rsettings["counter[Died]"] = to_string(counter[Person::Dead]);
	rsettings["counter[All]"] = to_string(sumCounter());

	if(push_data)
		push_graph_data(rsettings);

	saveData(rsettings);
	settings_mutex.unlock();
}

void Simulation::saveData(std::unordered_map<std::string, std::string>& line)
{
	if (save_file != nullptr)
	{
		for (auto it = line.begin(); it != line.end(); it++)
		{
			*save_file << it->first << "=" << it->second << ";";
		}
		*save_file << endl;
	}
}


Simulation::~Simulation()
{
	this->clearEntities();
	if (simulation_thread != nullptr)
		delete simulation_thread;
	if (own_settings)
		delete settings;
	if (save_file != nullptr)
	{
		//write data from save_file to disk
		ofstream file(save_data);
		file << save_file->str();
		file.close();
		save_file->clear();
		delete save_file;
	}
}

void Simulation::createChunks()
{
	if (chunks.size() > 0)
	{
		for (Chunk* chunk : chunks)
			delete chunk;
		chunks.clear();
	}
	Vecteur chunk_size = size / n_chunk;
	for (size_t i = 0; i < n_chunk; i++)
	{
		for (size_t j = 0; j < n_chunk; j++)
		{
			chunks.push_back(new Chunk({ i * chunk_size.x, j * chunk_size.y }, chunk_size));
		}
	}
	for (Entity* pe : this->univers)
	{
		vector<Chunk*> pe_chunks = getChunks(pe);
		for (Chunk* chunk : pe_chunks)
			chunk->addEntity(pe);
	}
}

/*
* init entities in the simulation with this->settings
*/
void Simulation::createEntities()
{
	for (size_t i = 0; i < stoi((*settings)["counter[Normal]"]); i++)
	{

		addPerson(new Person(randVecteur(size), randVecteur({4,4})-Vecteur(2,2), Person::Normal));

	}
	for (size_t i = 0; i < stoi((*settings)["counter[Recovered]"]); i++)
	{
		addPerson(new Person(randVecteur(size), randVecteur({ 4,4 }) - Vecteur(2, 2), Person::Recovered));
	}
	for (size_t i = 0; i < stoi((*settings)["counter[Infected]"]); i++)
	{
		addPerson(new Person(randVecteur(size), randVecteur({ 4,4 }) - Vecteur(2, 2), Person::Infected));
	}

}

void Simulation::start()
{
	if(simulation_thread!=nullptr)
		delete simulation_thread;
	iterations = 0;
	simulation_thread = new thread(simulationLoop,this);
}



void Simulation::simulationLoop(Simulation* simulation)
{
	while (simulation->running)
	{
		if (!simulation->paused)
		{
			simulation->update();
			if (simulation->push_data)
				simulation->pushData();
			wait(simulation->wait_time);
		}
		else
			wait(100);
	}
	cerr <<endl;
	cerr <<"Fin de la boucle de simulation apres " <<simulation->iterations <<" iterations !" <<endl;
}

void Simulation::stop()
{
	running = false;
}

void Simulation::pause()
{
	paused = true;
}

void Simulation::resume()
{
	paused = false;
}

void Simulation::join()
{
	if(simulation_thread->joinable())
		simulation_thread->join();
}

void Simulation::update()
{
	
	auto t_start = std::chrono::high_resolution_clock::now();
	settings_mutex.lock();
	for(Entity* pe : this->univers)
		pe->update();
	iterations++;
	sprites_updated = false;
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time = std::chrono::duration<double>(t_end - t_start).count();
	(*settings)["iteration_time"] = to_string(elapsed_time);
	settings_mutex.unlock();
}

void Simulation::checkWallsSettings()
{
	vector<Wall*> walls_already_simulated;
	vector<Wall*> walls_needed;
	for (Chunk* c : chunks)
		for (Entity* e : c->getEntities())
		{
			Wall* wall = dynamic_cast<Wall*>(e);
			if (wall && wall->isEnabled())
			{
				walls_already_simulated.push_back(wall);
			}
		}

	auto& rsettings = *settings;
	string walls = rsettings["walls"];

	//remove '[' and ']'
	walls = walls.substr(1, walls.size() - 2);
	vector<string> walls_str = split(walls, ",");
	for (string wall_str : walls_str)
		{
			try
			{
				//remove '(' and ')'
				vector<string> wall = split(wall_str, ":");
				if (wall.size() == 4)
				{
					Vecteur pos(stod(wall[0]), stod(wall[1]));
					Vecteur size(stod(wall[2]), stod(wall[3]));
					//if not wall with this properties in the simulation
					bool wall_exist = false;
					for (Wall* w : walls_already_simulated)
					{
						if (w->getPos() == pos &&  w->getSize() == size)
						{
							wall_exist = true;
							walls_needed.push_back(w);
							break;
						}
					}
					if (!wall_exist) for (Wall* w : walls_needed)
					{
						if (w->getPos() == pos && w->getSize() == size)
						{
							wall_exist = true;
							break;
						}
					}
					if (!wall_exist)
					{
						Wall* new_Wall = new Wall(pos, size);
						if(addWall(new_Wall))
							walls_needed.push_back(new_Wall);
					}
				}
			}
			catch (std::invalid_argument error)
			{
				cerr << "Invalid wall settings" << endl;
				cerr << "This may cause a crash or problem !" << endl;
				cerr << error.what() << endl;
			}
			
		}

	//delete all walls which are in the simulation but not in the settings
	//that means in walls_already_simulated but not in walls_needed*
	for (Wall* w : walls_already_simulated)
	{
		bool wall_exist = false;
		for (Wall* w2 : walls_needed)
		{
			if (w == w2)
			{
				wall_exist = true;
				break;
			}
		}
		if (!wall_exist)
		{
			removeEntity(w);
		}
	}

}

bool Simulation::addWall(Wall* w)
{
	//Check if the wall is in the simulation and add it to the simulation
	for (Chunk* c : getChunks(w))
		for (Entity* e : c->getEntities())
		{
			if (e != w && e->collision(w))
			{
				Wall* wall = dynamic_cast<Wall*>(e);
				if (wall && wall->isEnabled())
				{
					//error wall is in a wall
					delete w;
					cerr << "Wall in a wall" << endl;
					return false;
				}
				else
				{
					//move the entity
					Vecteur new_pos;
					while (this->getWallCollision(e))
					{
						new_pos = randVecteur(size);
						
					}
					e->setPos(new_pos);

				}

			}
		}
	this->addEntity(w);
}

void Simulation::addPerson(Person* p)
{
	//Check if the person is in the simulation but not in a wall and add it to the simulation
	//if it in a wall move randomly

	while (getWallCollision(p))
	{
		p->setPos(randVecteur(size));
	}
	this->addEntity(p);
}
