#include "Window.hpp"

void Window::startWindowLoop(Window* win)
{
	win->windowLoop();
}

void Window::windowLoop()
{
	window.create(videoMode, "Simulation");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(fps);



	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		for (sf::Shape* sprite : simulation->getSprites())
		{
			window.draw(*sprite);
		}
		if (showChunk)
			for (Chunk* chunk : simulation->getChunks())
			{
				window.draw(chunk->drawPerimeter());
			}


		window.display();

	}


	window.close();
}


Window::Window(Simulation* simulation, sf::VideoMode& _video_mode, unsigned int _fps, string _output_file) :simulation(simulation), fps(_fps), running(false), videoMode(_video_mode), showChunk(false)
{
	window_thread = nullptr;
}

Window::Window(Simulation* simulation, unsigned int _fps, const Vecteur& _window_size, string _output_file) : simulation(simulation), fps(_fps), running(false), videoMode(_window_size.x, _window_size.y), initial_size(_window_size), showChunk(false)
{
	window_thread = nullptr;
}

void Window::setShowChunk(bool state)
{
	showChunk = state;
}

Window::~Window()
{
	if (window_thread != nullptr)
		delete window_thread;
}

void Window::start()
{
	running = true;
	window_thread = new thread(startWindowLoop, this);
}

void Window::stop()
{
	running = false;
}

void Window::join()
{
	window_thread->join();
}

