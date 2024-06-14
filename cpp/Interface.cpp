#include "Interface.hpp"
#include "Simulation.hpp"

bool is_integer(const string& value)
{
	for (char c : value)
		if (!isdigit(c))
			return false;
	return true;
}

bool is_float(const string& value)
{
	bool point = false;
	for (char c : value)
	{
		if (c == '.')
		{
			if (point)
				return false;
			point = true;
		}
		else if (!isdigit(c))
			return false;
	}
	return point;
}


Interface::Interface(Simulation* simulation, unsigned int fps)
	: videoMode(800, 600), fps(fps), running(false), simulation(simulation)
{
}

Interface::Interface(Simulation* simulation, const sf::VideoMode& videoMode)
	: videoMode(videoMode), fps(30), running(false), simulation(simulation)
{
}

Interface::Interface(Simulation* simulation, const sf::VideoMode& videoMode, unsigned int fps)
	: videoMode(videoMode), fps(fps), running(false), simulation(simulation)
{
}

Interface::~Interface()
{
	stop();
	join();

}

void Interface::setSimulation(Simulation* simulation)
{
	this->simulation = simulation;
}

void Interface::setFps(unsigned int fps)
{
	this->fps = fps;
}

void Interface::setVideoMode(const sf::VideoMode& videoMode)
{
	this->videoMode = videoMode;
}

void Interface::start()
{
	if (running)
		return;

	running = true;
	interface_thread = std::thread(startInterfaceLoop, this);
}

void Interface::join()
{
	if (!running || !interface_thread.joinable())
		return;

	interface_thread.join();
}

void Interface::stop()
{
	if (!running)
		return;

	running = false;
}

void Interface::startInterfaceLoop(Interface* _interface)
{
	_interface->interfaceLoop();
}

void Interface::interfaceLoop()
{

	window.create(videoMode, "UserInterfacePandemie");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(fps);


	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				running = false;
			}

		}

		
		if (update_frame())
		{
			window.clear(sf::Color::Black);
			draw_frame();
			window.display();
		}
	}

	window.close();
	running = false;
}
