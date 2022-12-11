#include "worker.h"
#include "master_control.h"

Worker::Worker(MasterControl* master)
    :
    master(master),
    thread(&Worker::Work, this)
{}

Worker::~Worker()
{
    Die();
}

void Worker::SetTransput(std::vector<double> input, double* output)
{
    {
        std::lock_guard guard(mutex);
        this->input = input;
        this->output = output;
    }
    conditionVariable.notify_one();
}

void Worker::Die()
{
    {
        std::lock_guard guard(mutex);
        die = true;
    }
    conditionVariable.notify_one();
}

void Worker::Work()
{
    std::unique_lock lock{ mutex };

    while (true)
    {
        conditionVariable.wait(lock,
            [this]()
            {
                return output != nullptr || die;
            }
        );

        if (die)
        {
            break;
        }

        for (auto& data : input)
        {
            *output += sin(data) + cos(data);
        }

        input.clear();
        output = nullptr;

        master->AlertDone();
    }
}