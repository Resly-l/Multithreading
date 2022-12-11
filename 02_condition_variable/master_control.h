#pragma once
#include <condition_variable>
#include <mutex>

class MasterControl
{
private:
	std::condition_variable conditionVariable;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock;
	int numWorkers;
	int numDoneWorkers = 0;

public:
	MasterControl(int numWorkers);

public:
	bool IsAllDone() const;
	void AlertDone();
	void WaitForAllDone();
};