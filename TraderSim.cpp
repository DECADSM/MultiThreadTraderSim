#include "TraderSim.h"
#include <vector>
#include <random>
#include <mutex>
#include <thread>

std::mutex mtx;
//command line to compile
//g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 TraderSim.cpp -o TraderSim
void print()
{
    std::cout << "Hello there from the thread." << std::endl;
}
/*
void AddOrder(std::vector<TradeInfo> &queue, TradeInfo _info)
{
    mtx.lock();
    queue.push_back(_info);
    mtx.unlock();
}
//*/

void AddOrder(std::vector<TradeInfo> &queue, int id, double price, OrderType t, int quantity)
{
    std::lock_guard<std::mutex> lock(mtx); //Lock the shared resource, locking it from every thread but the one currently using it
    queue.emplace_back(TradeInfo(id, price, t, quantity));
}

int main()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> coinflip(0, 1); // define the range int
    std::uniform_real_distribution<> dist(0, 100);

    std::vector<Trader> traders;
    std::vector<std::thread> threads;
    std::vector<TradeInfo> OrderStack;

    int num_trader = 10; //number of traders to make
    
    for(int i = 0; i < num_trader; i++)
    {
        double price = dist(gen);
        
        threads.emplace_back(AddOrder, std::ref(OrderStack), i, price);
    }
    /*
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
    //*/
    //Join all the threads back
    for(int i = 0; i < num_trader; i++)
    {
        if(threads[i].joinable())
        {
            OrderStack[i].Print();
            threads[i].join();
        }
    }
    /*
    for(TradeInfo t : OrderStack)
    {
        t.Print();
    }
    //*/
    /*
    std::thread t(print);
    if(t.joinable())
        t.join(); //*/

    std::cout << "Main thread finished" << std::endl;
    return 0;
}
