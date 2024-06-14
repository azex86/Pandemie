#pragma once
#include <thread>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>
#include <atomic>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Vecteur.hpp"
#include "Chunk.hpp"
#include "Entity.hpp"
#include "Person.hpp"
#include "Wall.hpp"
#include "graph.h"
#include "sleep.h"
#define NOMINMAX
#include <SFML/Graphics.hpp>

using namespace std;

class Entity;
class Chunk;
class Person;
class Wall;
#define COUNTER_SIZE 4

class Simulation
{
private:
	thread* simulation_thread;
	size_t n_thread;
	atomic<bool> running;
	atomic<bool> paused;
	size_t wait_time;
	std::mutex settings_mutex;
	std::unordered_map<std::string, std::string>* settings;
	bool own_settings;
	string save_data = "";
	stringstream* save_file = nullptr;
	bool push_data = true;

	size_t iterations;
	size_t counter[COUNTER_SIZE];
	size_t sumCounter()const;
	void initCounter();

	vector<Entity*> univers;
	vector<Chunk*> chunks;
	Vecteur size;
	size_t n_chunk;

	vector<sf::Shape*> sprites;
	atomic<bool> sprites_updated;

	static void simulationLoop(Simulation* simulation);
	void update();


	//check the settings["walls"] and add  walls to the simulation if necessary
	//settings["walls"] is like "[,,,,]" where each element is a wall like "pos.x1;pos.y1;size.x2;size.y2"
	//it assume that settings_mutex is locked
	void checkWallsSettings();
	
	//add a Wall to the simulation and check its location, collision with other walls and other entity, correct it if necessary
	//if collision with other walls, the wall is not added
	//if collision with other entity, the wall is added but the entity is moved
	bool addWall(Wall* w);
	//add a person to the simulation and check its position, collision with walls, correct it if necessary
	void addPerson(Person* p);


public:
	Simulation();
	Simulation(std::unordered_map<std::string, std::string>& settings, const Vecteur& _size = {100,100}, size_t _n_chunk = 10,size_t _n_thread = 1);
	~Simulation();

	void createChunks();
	void createEntities();

	void start();
	void stop();
	void pause();
	void resume();
	void join();

	void setSettings(std::unordered_map<std::string, std::string>& settings);
	std::unordered_map<std::string, std::string>* getSettings(void);
	mutex& getSettingsMutex(void);
	//called when setting is changed by another thread
	void updateSettings(void);

	void pushData();
	void saveData(std::unordered_map<std::string, std::string>& line);

	void updateEntityChunks(Entity* e);
	void addEntity(Entity* e);
	void removeEntity(Entity* e);
	void clearEntities();

	//return all chunks that intersect with the point
	vector<Chunk*> getChunks(const Vecteur& pos);
	//return all chunks that intersect with the rectangle
	vector<Chunk*> getChunks(const Vecteur& pos, const Vecteur& size);
	//return all chunks that intersect with the entity
	vector<Chunk*> getChunks(const Entity& e);
	//return all chunks that intersect with the entity
	vector<Chunk*> getChunks(const Entity* e);
	//return all chunks
	const vector<Chunk*>& getChunks()const;

	//check if the rectangle is in collision with any Wall
	bool getWallCollision(const Vecteur& pos,const Vecteur& size);
	bool getWallCollision(const Entity* e);
	vector<sf::Shape*> getSprites();
	const Vecteur& getSize()const;
	size_t getIterations()const;
	size_t getCounter(unsigned short s)const;
	size_t* getCounter();
	size_t getSumCounter()const;

	void setSaveData(string filename);
	void setPushData(bool push);

};

