#ifndef FARM_H
#define FARM_H
#include <queue>
#include "task.h"
#include "attack.h"
#include "global.h"

#include <mutex>
#include <thread>
#include <vector>

using namespace std;


// A collection of tasks that should be performed in parallel. 
class Farm {
public:

	void add_task(Task *task);				// Add a task to the farm. The task will be deleted once it has been run.
	void run(int userThread);				//Run all the tasks in the farm. (passes in user defined threds)

private:
	queue<Task *> myq;
	mutex qc;
};

#endif
