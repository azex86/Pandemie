#pragma once
#include "str.h"
#include <thread>
#include <unordered_map>
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <functional>
#include "help.h"
using namespace std;

extern atomic<bool> stopping_user_input;
extern thread* user_input_thread;
//f : lambda function called when data is received from stdin
//start a thread that will read data from stdin to settings and call f
void start_user_input(unordered_map<string, string>* settings, mutex* setting_mutex, const std::function <void(void)>& f);
void stop_user_input();
void join_user_input();