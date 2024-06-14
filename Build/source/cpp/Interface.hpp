#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
class Simulation;
class Component;

class Interface
{
private:
	sf::RenderWindow window;
	sf::VideoMode videoMode;
	unsigned int fps;
	bool running;


	Simulation* simulation;


	std::unordered_map<std::string, Component*> buttons;
	std::vector<Component*> cursors;

	std::thread interface_thread;

	static void startInterfaceLoop(Interface* _interface);
	void interfaceLoop();

	//return true if there is a change
	bool update_frame();
	void draw_frame();
	bool update_value();
	void addCursor(Component* _cursor);

public:
	Interface(Simulation* simulation, unsigned int fps = 30);
	Interface(Simulation* simulation, const sf::VideoMode& videoMode);
	Interface(Simulation* simulation, const sf::VideoMode& videoMode, unsigned int fps = 30);

	~Interface();

	void setSimulation(Simulation* simulation);
	void setFps(unsigned int fps);
	void setVideoMode(const sf::VideoMode& videoMode);

	

	void start();
	void join();
	void stop();
};

// Path: cpp/Interface.cpp