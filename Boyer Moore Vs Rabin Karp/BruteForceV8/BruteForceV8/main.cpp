/*
	Program: password cracking program with passwords generated.
	Programer : Michael J Kleinman
	Program name : BruteForce

	Farm template from : Task - based parallelism example, Adam Sampson <a.sampson@abertay.ac.uk>
	hashing libary for sha256 https: http://create.stephan-brumme.com/hash-library/ 
*/

	//libraries
	#include <iostream>
	#include <string>
	#include <chrono>	//clock	
	#include <fstream>	//csv file

	//header files
	#include "farm.h"
	#include "task.h"
	#include "attack.h"
	#include "listener.h"
	#include "global.h"

	//hashing libaries
	#include "sha256.h"

	//#include "picosha2.h"	// hashing libary sha256 https://github.com/okdshin/PicoSHA2
	//used for hashing password & attempt http://create.stephan-brumme.com/hash-library/

	using namespace std;

	//required for an accurate clock
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	using std::chrono::steady_clock;

	//required for testing times
	typedef std::chrono::steady_clock the_clock;

	bool finished = false;		//global variable, triggred by consumer thread. Ends farm

	int main()
	{
		string attempt = "";		//string created by brute function
		string pword = "";			//password to crack
		string orig = "";			//saves password before hashing
		int a = 0;					//limits of character array for each task
		int b = 16;

		int limit = 5;				//limit for ammount of tests that should be run	

		int userThread = 0;								//ammount of threads that are run 
		int n = thread::hardware_concurrency();			//displays recomended threads (computer dependent)

		attack attackObj;			//object for the attack class
		vector <thread> vec_thr;	//thread for brute to run on
		Farm f;						//running a load of tasks using a farm
		listener listenerObj;
	
		SHA256 hashPword;			//sha256 hash object
		ofstream myFile;			//writing output to file

		cout << "bruteForceV8\n--------------------------------------\nPassword Cracking on CPU\n--------------------------------------\n" << endl;
		

		//user defines how many threads the program runs on
		cout << "Suggested thread total : " << n << endl;
		cout << "\nPlease enter the ammount of threads : ";
		cin >> userThread;

		//error checking
		if (userThread < 2)
		{
			do
			{
				cout << "\n***Error : Thread amount must be over 2";
				cout << "\nPlease enter the thread amount : ";
				cin >> userThread;
				cout << "\n";
			} while (userThread < 2);
		}
		//

		//defines how many times the crack is run for farm & serial
		cout << "Please enter the ammount of tests to run : ";
		cin >> limit;
	
		//error checking
		if (limit < 1)
		{
			do
			{
				cout << "\n***Error : test amount must at least 1";
				cout << "\nPlease enter number of test : ";
				cin >> limit;
				cout << "\n";
			} while (limit < 1);
		}

		//entering a password to crack (1-5 no special chars)
		cout << "Please set the password : ";
		cin >> pword;
	
		//error checking
		if (pword.length() < 1 || pword.length() > 5)
		{
			do
			{
				cout << "\nError: Please enter a password between 1 - 8 characters long" << endl;
				cout << "Please set the password : ";
				cin >> pword;

			} while (pword.length() < 1 || pword.length() > 5);	//only runs if the password is between 1 & chars long

		}

		myFile.open("BruteForceV8-Farm.csv"); 	//putting timings in a CSV File
		myFile << "BruteForceV78 - Password = " << pword << "\n";
		
		orig = pword;
		pword = hashPword(pword);		//hash the password

		SignalContainer mainContainer;								//stores the components required for the condition variable
		listenerObj.signalContainer = &mainContainer;				//pointer to the objects condition variable


		myFile << "Hashing: sha256";
		myFile << "\nTotal Number of Tests : ," << limit << "\nTotal Thread count : ," << userThread << "\n";
		myFile << "\nThreaded\n";
		myFile << "\nTest Number,Timings \n";

		cout << "\n------\nFarm\n------\n" << endl;
		///Threaded
		for (int i = 0; i < limit; i++)
		{
			myFile << i << ",";
			
			while (a <= 64)
			{
				f.add_task(new attack(pword, attempt, a, b, &mainContainer));				//adding the new set of limits to the que

				a = a + 16;
				b = a + 16;						//splits the work into sections		thread1 : 0-16 | thread2 : 16-32 | thread3 : 32-48 | thread4 : 48-64  (for the Que)
			}

			cout << "Test number : " <<i <<"\nCracking Password...\n";
			the_clock::time_point start = the_clock::now();
		
			listenerObj.run();					//runs listener (endAll) from the listener class function
			f.run(userThread);					//runs the farm
			listenerObj.consumer.join();		//joins the listener when completed

			the_clock::time_point end = the_clock::now();
			
			auto time_taken = duration_cast<milliseconds>(end - start).count();				//displays time, threaded
			cout << "\nPassowrd "<<orig<<" found\nThreaded Time: " << time_taken << " ms." << endl<<endl;
			
			myFile << time_taken << "\n";
			
			//resets the values for testing purposes
			a = 0;
			b = 0;
			finished = false;
			listenerObj.signalContainer->endThreads = false;
		}
		///
		myFile.close();
		
		///no thread
		myFile.open("BruteForceV8-Serial.csv"); 	//putting timings in a CSV File
		myFile << "BruteForceV78 - Password = '"<< orig<<"'\n";
		myFile << "Hashing: sha256";
		myFile << "\nTotal Number of Tests : ," << limit << "\nTotal Thread count : ," << userThread << "\n";
		myFile << "\nTest Number,Timings \n";

		cout << "\n------\nSerial\n------\n" << endl;
		for (int i = 0; i < limit; i++)
		{
			cout << "\nTest number : " << i << "\nCracking Password...\n";
			myFile <<"\n"<< i << ",";
			the_clock::time_point start2 = the_clock::now();
			attackObj.bruteNoThread(pword, attempt);
			the_clock::time_point end2 = the_clock::now();

			auto time_taken2 = duration_cast<milliseconds>(end2 - start2).count();			//displays time, series
			cout << "\nPassowrd " << orig << " found\nSerial Time: " << time_taken2 << " ms." << endl << endl;
			myFile << time_taken2 << ",";
		}

		myFile.close();
		///
		return 0;
	}

