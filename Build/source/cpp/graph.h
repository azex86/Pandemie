#pragma once
#include "str.h"
#include <unordered_map>
#include <iostream>
#include <atomic>
#include <mutex>
#include <functional>

using namespace std;








bool startGraph(string sourceGraph = "python IHM/main.py");
void stopGraph();
void joinGraph();

void write_data_to_graph(const string& data);


//push data to the graph (IHM)
void push_graph_data();
//push data to the graph (IHM)
//using a variadic args
template<typename T, typename ...Args>
inline void push_graph_data(const string& first_name_arg,const T& first_arg, const Args & ...rest)
{
	write_data_to_graph(first_name_arg + "=" + to_string(first_arg) + ";\n");
	push_graph_data(rest...);
}

//push data to the graph (IHM)
//using a variadic args
void push_graph_data(const std::vector<string>& args_names,const std::vector<std::string>& args);
//push data to the graph (IHM)
//using a variadic args
void push_graph_data(const unordered_map<string, string>& data);