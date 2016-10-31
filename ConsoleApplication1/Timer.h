#pragma once
#include <time.h>
#include <ctime>
#include <iostream>

using namespace std;

class stopwatch
{
private:
	std::clock_t start;
public:
	stopwatch() : start(std::clock()) {} //start counting time
	~stopwatch();
};

stopwatch::~stopwatch()
{
	clock_t total = clock() - start; //get elapsed time
	cout << " " << endl;
	cout << "total of ticks for this activity: " << total << endl;
	cout << "in seconds: " << double(total) / CLOCKS_PER_SEC << endl;
};