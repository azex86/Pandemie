#include "help.h"
#include <fstream>
#include <iostream>

std::string help_content_default = "BY AZE\n"
"--save [file(.csv)]=> define a name of the file to save the statistics\n"
"--nbthread [int] => define the number of thread to use to execute the calculations\n"
"--show => show the simulation\n"
"--show-chunk => show the chunks\n"
"--stop-when-pandemic-end => stop the simulation when the pandemic is over\n"
"--no-stop-when-pandemic-end => do not stop the simulation when the pandemic is over\n"
"--no-show => do not show the simulation\n"
"--no-graph => do not show the graph\n"
"--graph  => show the graph\n"
"--graph-script [file(.py)] : python main.py => define the command of the script to use to show the graph\n"
"--size-map [int] [int] : 1000 1000 => define the size of the map\n"
"keys available : "
"wait_time, live, counter[All], counter[Infected], counter[Normal], counter[Recovered], counter[Died], rules[infection_time], rules[infection_range], rules[infection_rate], rules[immunity_time]\n"
"wait_time : 50 : time to wait between each iteration in milliseconds\n"
"live : 2 : 1 to continue execution, 2 to pause, 0 to stop\n"
"counter[All] : default 10000 : total number of people the sum of all others counter should be counter[All]\n"
"counter[Infected] : 1 : number of infected people\n"
"counter[Normal] : 9999 : number of healthy people\n"
"counter[Recovered] : 0 : number of people healed\n"
"counter[Died] : 0 : number of people dead\n"
"rules[infection_time] : 300 : time to heal after infection\n"
"rules[infection_range] : 2 : contamination distance\n"
"rules[infection_rate] : 1 : contamination probability\n"
"rules[immunity_time] : 900 : time to lose immunity\n"
"rules[quarantine] : 0 : probability of confinement\n"
"rules[confinement] : 0 : probability of confinement\n"
"rules[quarantine_time] : 15 : time between infection and quarantine\n"
"walls : list of walls in the form of a list [wall,,,,] and wall in the form pos.x:pos.y:size.x:size.y\n"
;

void help_content()
{
	using namespace std;
	try
	{
		ifstream in("help.txt");

		string content = "";
		string line;
		while (getline(in, line))
		{
			content += line + "\n";
		}

		cout << content <<endl;
	}catch(exception e)
	{
		cerr << "Error during reading help.txt\n";
		cout <<help_content_default <<endl;
	}

}
