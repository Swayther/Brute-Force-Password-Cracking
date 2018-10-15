#pragma once
#include "task.h"
#include "global.h"

//hashing libaries
#include "sha256.h"		//used for hashing password & attempt http://create.stephan-brumme.com/hash-library/
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>


struct SignalContainer
{
	//variables for the CONDITION VARIABLE are in a struct so they can be accessed by the listener class (via a pointer)
	std::mutex endThreads_mutex;
	bool endThreads = false;
	std::condition_variable endThreads_cv;
};

class attack : public Task
{
public:
	attack(std::string pword, std::string attempt, int a, int b, SignalContainer* container);
	attack();
	~attack();

	void run();														//runs everything in the que through the farm (producer thread)
	void bruteNoThread(std::string pword, std::string attempt);		//runs the bruteforce attack in serial, acts as a control
	void check(std::string attempt);								//compares hashes of attempt and the password (consumer thread)
	bool checkNoThread(std::string pword,std::string attempt);
	
	SignalContainer* signalContainer;			//signal pointer that lets the listener class access the origional signal container
	


private:
	std::string pword;
	std::string	attempt;
	int a;
	int b;

	SHA256 attemptHash;

	bool found = false;				//determines when the password was found (no thread)

	std::mutex tries_mutex;
	int tries = 0;					//calculates the ammount of tries it required before the password was broken (potential race condition)

	std::string hashAttempt = "";	//hash if the attempt

	const char alpha[64]			//the char array that the attempts are created from
	{
		'A', 'B', 'C', 'D', 'E', 'F', 'G','H', 'I', 'J', 'K', 'L', 'M', 'N','O', 'P', 'Q', 'R', 'S', 'T', 'U','V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g','h', 'i', 'j', 'k', 'l', 'm', 'n','o', 'p', 'q', 'r', 's', 't', 'u','v', 'w', 'x', 'y', 'z',
		'0','1','2','3','4','5','6','7','8','9','.',' '
	};

};

