#include "TraderSim.h"
#include <thread>
#include <vector>
#include <random>
//command line to compile
//g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 TraderSim.cpp -o TraderSim
void print()
{
    std::cout << "Hello there from the thread." << std::endl;
}

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 100); // define the range

    std::vector<Trader> traders;
    std::vector<std::thread> threads;
    int num_trader = 10; //number of traders to make
    for(int i = 0; i < num_trader; i++)
    {
        traders.emplace_back("Trader " + std::to_string(i));
    }
    for(int i = 0; i < num_trader; i++)
    {
        //using a lambda to pass a member function to a thread
        //threads.emplace_back([&traders, i]() { traders[i].Activate(); traders[i].PrintActive(); });
        int numA = distr(gen);
        int numB = distr(gen);
        threads.emplace_back([&traders, i, numA, numB]() { traders[i].Activate(); traders[i].PrintActive(); std::cout << traders[i].AddNumber(numA, numB) << std::endl; });
    }

    for(int i = 0; i < num_trader; i++)
    {
        if(threads[i].joinable())
        {
            threads[i].join();
        }
    }
    /*
    std::thread t(print);
    if(t.joinable())
        t.join(); //*/

    std::cout << "Main thread finished" << std::endl;
    return 0;
}
