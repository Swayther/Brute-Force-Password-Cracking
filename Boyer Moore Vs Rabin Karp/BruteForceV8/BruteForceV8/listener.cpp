#include "listener.h"

listener::listener()
{
}

listener::~listener()
{
}

void listener::endAll()		//ends all the threads running if the password is found
{
	std::unique_lock<std::mutex> lck(signalContainer->endThreads_mutex);
	while (!signalContainer->endThreads) signalContainer->endThreads_cv.wait(lck); //while loop in place to ensure consumer waits for producer

	finished = true;
}

void listener::run()
{
	consumer = std::thread(&listener::endAll, this);	//runs the listener 
}