/*
 * condQueue.cpp
 *
 * Info: http://thisthread.blogspot.com/2013/08/condition-variable-on-queue.html
 */

#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>

class Conditional
{
private:
    std::queue<int> queue_;
    std::condition_variable cond_;
    std::mutex mutex_;

public:
    void producer()
    {
        for(int i = 10; i >= 0; --i)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            std::cout << "producing " << i << std::endl;
            queue_.push(i);
            cond_.notify_one();
        }
    }

    void consumer()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while(queue_.empty())
            {
                std::cout << "waiting queue" << std::endl;
                cond_.wait(lock);
//                if(queue_.empty())
//                    std::cout << "spurious ";
            }
            int message = queue_.front();
            queue_.pop();

            if(!message)
            {
                std::cout << "terminating" << std::endl;
                return;
            }

            std::cout << "consuming " << message << std::endl;

//            if(message == 5)
//            {
//                lock.unlock();
//                std::this_thread::sleep_for(std::chrono::microseconds(50));
//            }
        }
    }
};

int main()
{
    Conditional c;

    std::thread tc(&Conditional::consumer, &c);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::thread tp(&Conditional::producer, &c);

    tc.join();
    tp.join();

    std::cout << "done" << std::endl;
}
