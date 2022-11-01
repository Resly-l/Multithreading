#include <iostream>
#include <vector>
#include <array>
#include <thread>

#include "timer.h"
#include "rng.h"

#define NUM_DATASET 4
#define DATASET_SIZE 2000000

void WorkHard(std::array<double, DATASET_SIZE>& dataset, double& sum)
{
	for (int i = 0; i < DATASET_SIZE; i++)
	{
		sum += sin(dataset[i]) + cos(dataset[i]);
	}
}

int main()
{
	Timer<Milliseconds> timer;
	std::vector<std::thread> threads;

	std::vector<std::array<double, DATASET_SIZE>> datasets(NUM_DATASET);

	std::minstd_rand rng;
	for (auto& dataset : datasets)
	{
		std::ranges::generate(dataset, rng);
	}

	// When CPU reads value from memory(RAM) it doesn't read the exact size of variable.
	// Size of bits read is determinded by CPU cache which is usually 64bytes
	// So first thread will be reading all elements of sum[] to it's cache
	// When second thread reads sum[1], since now two threads are accessing same cache line,
	// CPU will syncronize this value in hardware level
	double sum[NUM_DATASET] = {};

	struct BigBoii
	{
		double sum = 0.0;
		char padding[56];
	};

	BigBoii bb[4] = {};

	timer.Mark();
	for (int i = 0; i < NUM_DATASET; i++)
	{
		threads.push_back(std::thread{ WorkHard, std::ref(datasets[i]),  std::ref(bb[i].sum) });
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::cout << "result : " << bb[0].sum + bb[1].sum + bb[2].sum + bb[3].sum << std::endl;
	std::cout << "took " << timer.Peek() << " milliseconds" << std::endl;

	return 0;
}