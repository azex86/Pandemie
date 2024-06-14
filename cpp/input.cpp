#include "input.h"

atomic<bool> stopping_user_input(false);
thread* user_input_thread = nullptr;


void running_user_input(unordered_map<string, string>* settings, mutex* setting_mutex, const std::function <void(void)>& f)
{
	try {
		cout <<"running_com : en attente de commande...\n";
		cout <<"You can type \"exit\" to stop the program\n";
		cout <<"Or write [rule]=[value] and press enter in order to change the value of a rule\n";
		while (!stopping_user_input)
		{

			string line;
			//read line in cin
			getline(cin, line);
			cout << "Reception d'une ligne : \"" << line << '"' << endl;
			if (line == "exit")
			{
				setting_mutex->lock();
				(*settings)["live"] = "0";
				setting_mutex->unlock();

				cout << "running_com : demande de fin de programme...\n";
				break;
			}
			else if (line == "help")
			{
				cerr << help_content;
			}
			else {
				//split line
				auto param = split(line, ";");
				bool change = false;
				setting_mutex->lock();
				for (string p : param) {

					if (p.find("=") != std::string::npos)
					{
						string key = split(p, "=")[0];
						string value = split(p, "=")[1];
						(*settings)[key] = value;
						change = true;
					}
				}
				setting_mutex->unlock();
				if (change)
				{
					f();
				}

			}
		}
	}
	catch (exception& e)
	{
		cerr << "Erreur de communication : " << e.what() << endl;
		setting_mutex->unlock();
	}

	cout << "Fin de runnin_com\n";

}





void start_user_input(unordered_map<string, string>* settings, mutex* setting_mutex, const std::function <void(void)>& f)
{
	user_input_thread= new thread(running_user_input, settings, setting_mutex, f);
}

void stop_user_input()
{
	stopping_user_input = true;
}

void join_user_input()
{
	if (user_input_thread != nullptr)
		if (user_input_thread->joinable())
		{
			cout <<"You may need to type \"exit\" to stop the program\n";
			user_input_thread->join();
		}
}

