#include <iostream>
#include <memory>
#include "master_control.h"
#include "worker.h"
#include "dataset.h"
#include "timer.h"

#define NUM_WORKERS 4
#define NUM_DATASETS 4
#define DATASET_SIZE 5000000
#define SUBSET_SIZE DATASET_SIZE / 1000

struct CachelineDouble
{
	double value;
	char padding[64 - sizeof(double)];
};

int main()
{
	MasterControl masterControl(NUM_WORKERS);
	std::vector<std::unique_ptr<Worker>> workers;
	CachelineDouble sum[NUM_DATASETS] = {};
	auto datasets = Dataset<double>::GenerateDeterministicSets(NUM_DATASETS, DATASET_SIZE);

	for (size_t i = 0; i < NUM_WORKERS; i++)
	{
		workers.push_back(std::make_unique<Worker>(&masterControl));
	}

	Timer<Seconds> timer;
	timer.Mark();

	for (size_t i = 0; i < DATASET_SIZE; i += SUBSET_SIZE)
	{
		for (size_t j = 0; j < NUM_WORKERS; j++)
		{
			workers[j]->SetTransput({ datasets[j].begin() + i, datasets[j].begin() + i + SUBSET_SIZE }, &sum[j].value);
		}
		masterControl.WaitForAllDone();
	}

	std::cout << "time elapsed : " << timer.Peek() << std::endl;
	std::cout << sum[0].value + sum[1].value + sum[2].value + sum[3].value << std::endl;

	return 0;
}