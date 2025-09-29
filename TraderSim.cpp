#include "TraderSim.h"
#include <thread>
#include <vector>

void print()
{
    std::cout << "Hello there from the thread." << std::endl;
}

int main()
{
    std::vector<Trader> traders;
    std::vector<std::thread> threads;
    int num_trader = 10; //number of traders to make
    for(int i = 0; i < num_trader; i++)
    {
        Trader trader("Trader " + std::to_string(i));
        traders.push_back(trader);
        threads.emplace_back([&traders, i]() { traders[i].Activate(); traders[i].PrintActive(); });
    }

    for(int i = 0; i < num_trader; i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
    /*
    std::thread t(print);
    if(t.joinable())
        t.join(); //*/

    std::cout << "Main thread finished" << std::endl;
    return 0;
}
