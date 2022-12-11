#include "master_control.h"

MasterControl::MasterControl(int numWorkers)
	:
	lock(mutex),
	numWorkers(numWorkers)
{}

bool MasterControl::IsAllDone() const
{
	return numDoneWorkers == numWorkers;
}

void MasterControl::AlertDone()
{
	{
		std::lock_guard guard(mutex);
		numDoneWorkers++;
	}

	if (IsAllDone())
	{
		conditionVariable.notify_one();
	}
}

void MasterControl::WaitForAllDone()
{
	conditionVariable.wait(lock, [this]() { return numDoneWorkers == numWorkers; });
	numDoneWorkers = 0;
}