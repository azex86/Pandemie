#define NOMINMAX
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "sleep.h"
#include "graph.h"
#include "input.h"
#include "Simulation.hpp"
#include "Window.hpp"
#include "Lutin.hpp"
#include "Wall.hpp"
#include "Person.hpp"
#include "help.h"

using namespace std;

unordered_map<string, string> global_settings;
int NThread = 1;

int main(int argc, char** argv,char **envp)
{


	bool stop_when_pandemic_end = true;
	string save_data = "";
	bool show_simulation = true;
	bool show_chunk = false;
	bool show_graph = true;
	Vecteur size_map = {1000,1000};
	size_t n_chunk = 10;
	save_data = "simulation.csv";



#ifdef _DEBUG
	string graph_script_path = "python IHM/main.py";
#else
	string graph_script_path = "python main.py";
#endif // _DEBUG

	


	global_settings["wait_time"] = "50";
	global_settings["live"]= "2";
	global_settings["counter[All]"] = "10000";
	global_settings["counter[Infected]"] = "1";
	global_settings["counter[Normal]"] = "9999";
	global_settings["counter[Recovered]"] = "0";
	global_settings["counter[Died]"] = "0";
	global_settings["rules[infection_time]"] = "300";
	global_settings["rules[infection_range]"] = "2";
	global_settings["rules[infection_rate]"] = "1";
	global_settings["rules[immunity_time]"] = "900";
	global_settings["rules[quarantine]"] = "0";
	global_settings["rules[confinement]"] = "0";
	global_settings["walls"] = "[]";//"[0:475:475:50,525:475:475:50]";
	global_settings["rules[quarantine_time]"] = "15";
	//check arguments
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-h" || arg == "--help")
		{
			help_content();
			return 0;
		}
		else if (arg == "--nbthread")
		{
			i++;
			NThread = stoi(argv[i]);
		}
		else if (arg == "--save")
			save_data = argv[++i];
		else if(arg == "--show")
			show_simulation = true;
		else if (arg == "--show-chunk")
			show_chunk = true;
		else if (arg == "--stop-when-pandemic-end")
			stop_when_pandemic_end = true;
		else if (arg == "--no-stop-when-pandemic-end")
			stop_when_pandemic_end = false;
		else if (arg == "--no-show")
			show_simulation = false;
		else if (arg == "--no-show-chunk")
			show_chunk = false;
		else if (arg == "--no-graph")
			show_graph = false;
		else if (arg == "--graph")
			show_graph = true;
		else if (arg == "--size-map")
		{
			size_map.x = stoi(argv[++i]);
			size_map.y = stoi(argv[++i]);
		}
		else if(arg == "--n-chunk")
			n_chunk = stoi(argv[++i]);
		else if (arg == "--graph-script")
			graph_script_path = argv[++i];
		else if (arg == "--wall")
		{
			auto param = split(argv[++i], ",");
			string walls = "[";
			for (auto p : param)
			{
				auto wall = split(p, ":");
				walls += wall[0] + ":" + wall[1] + ":" + wall[2] + ":" + wall[3] + ",";
			}
			walls.pop_back();
			walls += "]";
			global_settings["walls"] = walls;
		}
		else
		{
			auto param = split(arg, "=");
			if (param.size() == 2)
			{
				global_settings[param[0]] = param[1];
			}
		}
	}
	

	//start communication with python process
	if (show_graph)
		show_graph = startGraph(graph_script_path);

	//start computing thread
	Simulation simulation(global_settings,size_map,n_chunk,NThread);
	simulation.setSaveData(save_data);
	simulation.setPushData(show_graph);
	//some entity to debug
	//simulation.addEntity(new Lutin(Vecteur(100, 100), Vecteur(50, 50), sf::Color::Red));

	simulation.start();



	//listen stin to change global_settings
	start_user_input(&global_settings, &simulation.getSettingsMutex(), [&] {simulation.updateSettings(); });

	//show computation result
	Window window(&simulation);
	window.setShowChunk(show_chunk);
	if(show_simulation)
		window.start();


	while (global_settings["live"]!="0")
	{
		wait(100);
		if (stop_when_pandemic_end && simulation.getCounter()[Person::Infected] == 0)
			break;
	}
	
	//stop all
	if (show_graph)
		stopGraph();
	if (show_simulation)
		window.stop();
	
	simulation.stop();
	
	if (show_simulation)
		window.join();
	if (show_graph)
		joinGraph();

	join_user_input();
	simulation.join();
	return 0;
}

