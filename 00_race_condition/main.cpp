/*
Race Condition Happens when multiple threads tries to access same memory
causing unexpected result. To prevent this, use std::mutex to lock memory.
When mutex is locked, no other threads can access that memory thus have to wait

+ When using mutex, mutex overhead should be taken into account
*/

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "timer.h"
#include "rng.h"

void WorkHard(double& sum, std::mutex& mutex)
{
	for (int i = 0; i < 2000000; i++)
	{
		mutex.lock();
		// std::lock_guard guard{ mutex };
		sum += (sin(RNG::Generate()) + cos(RNG::Generate()));
		mutex.unlock();
	}
}

int main()
{
	Timer<Milliseconds> timer;
	std::vector<std::thread> threads;
	std::mutex mutex;

	double sum = 0.0;

	timer.Mark();
	threads.push_back(std::thread{ WorkHard, std::ref(sum), std::ref(mutex) });
	threads.push_back(std::thread{ WorkHard, std::ref(sum), std::ref(mutex) });
	threads.push_back(std::thread{ WorkHard, std::ref(sum), std::ref(mutex) });
	threads.push_back(std::thread{ WorkHard, std::ref(sum), std::ref(mutex) });

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::cout << "result : " << sum << std::endl;
	std::cout << "took " << timer.Peek() << " milliseconds" << std::endl;

	return 0;
}