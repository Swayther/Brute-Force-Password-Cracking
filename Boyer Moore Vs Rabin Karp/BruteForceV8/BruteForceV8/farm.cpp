#include "farm.h"

void Farm::add_task(Task *task)
{
	myq.push(task);
}

void Farm::run(int userThread)
{
	vector <thread> myt;						//vector thread

	for (int i = 0; i < userThread; i++)
	{
		myt.emplace_back([&]
		{
			while (finished == false || !myq.empty())		//runs until variable is met or queue is empty
			{
				Task* t = nullptr;

				qc.lock();				//mutex protects task que

				if (!myq.empty())
				{
					t = myq.front();
					myq.pop();
				}
				qc.unlock();

				if (t != nullptr)
				{
					t->run();
					delete t;
				}
			}
		});
	}

	for (auto& x : myt) {
		x.join();			//joins once done
	}
}
