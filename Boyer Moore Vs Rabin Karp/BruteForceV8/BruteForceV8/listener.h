#pragma once
#include "attack.h"
#include "global.h"
#include <mutex>
#include <thread>
#include <vector>

//subclass of attack and acts as a listener to end the threads
class listener : public attack
{
public:
	listener();
	~listener();

	void run();									//function that starts and ends the endAll function
	void endAll();								//Ends all the threads once the passwords are found
	std::thread	consumer;
private:


};

