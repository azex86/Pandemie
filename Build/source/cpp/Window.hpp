#pragma once
#define NOMINMAX
#include <SFML/Graphics.hpp>
#include "Simulation.hpp"


class Window
{
	thread* window_thread;
	Simulation* simulation;
	sf::RenderWindow window;
	sf::VideoMode videoMode;
	Vecteur initial_size;
	unsigned int fps;
	atomic<bool> running;
	atomic<bool> showChunk;
	static void startWindowLoop(Window* win);
	void windowLoop();


public:
	Window(Simulation* simulation,sf::VideoMode& _video_mode, unsigned int fps = 30,string _output_file = "");
	Window(Simulation* simulation, unsigned int fps = 30,const Vecteur& _window_size = Vecteur(1000,1000),string _output_file = "");

	void setShowChunk(bool state);

	~Window();
	void start();
	void stop();
	void join();
};

