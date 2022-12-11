#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <span>
#include <vector>

class MasterControl;

class Worker
{
private:
    MasterControl* master;
    std::jthread thread;
    std::condition_variable conditionVariable;
    std::mutex mutex;

    std::vector<double> input;
    double* output = nullptr;
    bool die = false;

public:
    Worker(MasterControl* master);
    ~Worker();

public:
    void SetTransput(std::vector<double> input, double* output);
    void Die();

private:
    void Work();
};