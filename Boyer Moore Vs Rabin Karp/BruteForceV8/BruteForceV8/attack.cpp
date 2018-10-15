#include "attack.h"
#include <functional>



void attack::check(std::string attempt)
{
	attempt = attemptHash(attempt);

	if (attempt == pword)		//compares the hashes (pword Vs attempt)
	{
		std::cout <<"Tries : " << tries;

		//sends data to the listener 
		{
			std::unique_lock<std::mutex> lck(signalContainer->endThreads_mutex);		//unlock mutex
			signalContainer->endThreads = true;											//sents end threads to true
			signalContainer->endThreads_cv.notify_one();								//notifies endAll 
		}

	}
	else
	{
		std::unique_lock<std::mutex> lock(tries_mutex);
		tries++;
	}
}


void attack::run()
{
	for (int Ch1 = a; Ch1 <= b; Ch1++)		//1CHAR
	{
		attempt = std::string() + alpha[Ch1];
		check(attempt);
		if (finished == true || signalContainer->endThreads == true)
		{
			return;
		}
		for (int Ch2 = 0; Ch2 <= 64; Ch2++)		//2CHAR
		{
			attempt = std::string() + alpha[Ch1] + alpha[Ch2];
			check(attempt);
			if (finished == true || signalContainer->endThreads == true)
			{
				return;
			}
			for (int Ch3 = 0; Ch3 <= 64; Ch3++)		//3CHAR
			{
				attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3];
				check(attempt);
				if (finished == true || signalContainer->endThreads == true)
				{
					return;
				}
				for (int Ch4 = 0; Ch4 <= 64; Ch4++)		//4CHAR
				{
					attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3] + alpha[Ch4];
					check(attempt);
					if (finished == true || signalContainer->endThreads == true)
					{
						return;
					}
					for (int Ch5 = 0; Ch5 <= 64; Ch5++)		//5CHAR
					{
						attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3] + alpha[Ch4] + alpha[Ch5];
						check(attempt);
						if (finished == true || signalContainer->endThreads == true)
						{
							return;
						}
					}
				}
			}
		}
	}
}


bool attack::checkNoThread(std::string pword,std::string attempt)
{
	attempt = attemptHash(attempt);		//hashes attempt

	if (attempt == pword)				//compares hashes
	{
		std::cout <<"Tries : " << tries;
		return true;	
	}
	else
	{
		std::unique_lock<std::mutex> lock(tries_mutex);	//mutex prevents race condition with the tries variable
		tries++;
		return false;
	}
}


void attack::bruteNoThread(std::string pword, std::string attempt)
{
	tries = 0;	//reset tries counter

	for (int Ch1 = 0; Ch1 <= 64; Ch1++)		//1CHAR
	{
		attempt = std::string() + alpha[Ch1];
		found = checkNoThread(pword, attempt);
		if (found == true)
		{
			return;
		}
		for (int Ch2 = 0; Ch2 <= 64; Ch2++)		//2CHAR
		{
			attempt = std::string() + alpha[Ch1] + alpha[Ch2];
			found = checkNoThread(pword, attempt);
			if (found == true)
			{
				return;
			}
			for (int Ch3 = 0; Ch3 <= 64; Ch3++)		//3CHAR
			{
				attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3];
				found = checkNoThread(pword, attempt);
				if (found == true)
				{
					return;
				}
				for (int Ch4 = 0; Ch4 <= 64; Ch4++)		//4CHAR
				{
					attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3] + alpha[Ch4];
					found = checkNoThread(pword, attempt);
					if (found == true)
					{
						return;
					}	
					for (int Ch5 = 0; Ch5 <= 64; Ch5++)		//5CHAR
					{
						attempt = std::string() + alpha[Ch1] + alpha[Ch2] + alpha[Ch3] + alpha[Ch4] + alpha[Ch5];
						found = checkNoThread(pword, attempt);
						if (found == true)
						{
							return;
						}
					}
				}
			}
		}
	}
}


//overloaded constructor
attack::attack(std::string password1, std::string attempt1, int a1, int b1, SignalContainer* container)
{
	signalContainer = container;
	pword = password1;
	attempt = attempt1;
	a = a1;
	b = b1;
}

attack::attack()		//constructor
{
}

attack::~attack()
{
}
