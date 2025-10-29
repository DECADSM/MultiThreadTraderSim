#include "TraderSim.h"

#include <random>


std::mutex mtx;
//command line to compile
//g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 TraderSim.cpp -o TraderSim

OrderType RandomOrder()
{
    std::random_device rd;
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> coinflip(0, 1); // Buy/Sell coinflip

    if(coinflip(gen) == 0)
        return OrderType::BUY;
    
    return OrderType::SELL;
}

int main()
{
    /* 
    int jobsNum;
    std::cout << "Enter the number of jobs: ";
    std::cin >> jobsNum;
    Market stock(jobsNum);
    //*/
    std::random_device rd;
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> orders(10, 100); // Number of orders to make
    Market stock(std::thread::hardware_concurrency());
    for(int i = 0; i < orders(gen); i++)
    {
        stock.enqueue([i, gen, &stock]() mutable { 
            //create the order here for the thread to calculate out
            OrderType type = RandomOrder();
            std::uniform_real_distribution<> prices(1, 10000);
            std::uniform_int_distribution<> stock_amt(1, 1000);

            TraderInfo info(std::this_thread::get_id(), prices(gen), type, stock_amt(gen));
            stock.AddOrder(info);
        });
    }
    return 0;
}
