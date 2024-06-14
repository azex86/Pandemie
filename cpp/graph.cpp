#include "graph.h"
#include <boost/process.hpp>

namespace bp = boost::process;

bp::child process;
bp::opstream pipe_stream;
atomic<bool> stopping_com(false);



bool startGraph(string sourceGraph)
{
	try {
		process = bp::child(sourceGraph, bp::std_in < pipe_stream, bp::std_out > stdout, bp::std_err > stderr);
	}
	catch (exception& e)
	{
		cerr << "Erreur lors du lancement du graph : " << e.what() <<" command : " <<sourceGraph << endl;
		return false;
	}

	return true;
}

void stopGraph()
{
	if (process.running())
	{
		pipe_stream << "exit\n";
		pipe_stream.flush();
		pipe_stream.close();
	}
}

void joinGraph()
{
	if (process.running())
		process.wait();
}

void write_data_to_graph(const string& data)
{
	if (!process.running())
		return;
	pipe_stream << data;
	pipe_stream.flush();
}


void push_graph_data()
{
	if (!process.running())
		return;
	pipe_stream << endl;
	pipe_stream.flush();
}

void push_graph_data(const std::vector<string>& args_names, const std::vector<std::string>& args)
{
	if (args_names.size() == args.size())
		throw "args_names and args must have the same size";
	if (!process.running())
		return;
	for (size_t i = 0; i < args.size(); i++)
	{
		pipe_stream << args_names[i] << '=' << args[i] << ';';
	}
	pipe_stream << endl;
	pipe_stream.flush();
}

void push_graph_data(const std::unordered_map<std::string, std::string>& args)
{
	if (!process.running())
		return;
	for (auto& arg : args)
	{
		pipe_stream << arg.first << '=' << arg.second << ';';
	}
	pipe_stream << endl;
	pipe_stream.flush();
}



