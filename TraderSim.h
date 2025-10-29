#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <queue>


class Trader
{
    private:
        std::string _name;
        float _balance = 0;
        bool _active = false;
    public:
        Trader(std::string name) : _name(name)  { }
        Trader(std::string name, float balance) : _name(name), _balance(balance)  { }
        void Activate() { _active = true; }
        void Deactivate() { _active = false; }
        void PrintActive() { std::cout << GetName() << " is " << std::to_string(GetActive()) << std::endl; }
        int AddNumber(int a, int b) { return a + b; }
        bool GetActive() { return _active; }
        std::string GetName() { return _name; }
};

enum OrderType
{
    BUY,
    SELL,

};



struct TraderInfo 
{
    public:
    TraderInfo(std::thread::id id, double price, OrderType t, int q) : traderID(id), _price(price), type(t), quantity(q) { } 
    std::thread::id traderID;
    double _price;
    OrderType type;
    int quantity;
    std::string PrintOrderType(OrderType o)
    {
        if(o == OrderType::BUY)
            return "Buy";
        
        return "Sell";
    }
    void Print()
    {
        std::cout << "Trader " << traderID << " made a " << PrintOrderType(type) << " trade with " << quantity << " stocks with this price: " << _price << std::endl;
    }
};

class Market
{
    public:
        Market(int amt)
        {
            //thread check
            if(static_cast<unsigned int>(amt) > std::thread::hardware_concurrency())
            {
                std::cout << "Asked for too many threads. Current hardware can support about " << std::thread::hardware_concurrency() << ". Please check your hardware for proper thread counts.";
                return;
            }
            
            //creating worker threads
            for(int i = 0; i < amt; i++)
            {
                while(true)
                {
                    std::function<void()> task;
                    //blocking off locking code
                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        //Waiting for a new task or everything is stopped
                        cv.wait(lock, [this] { return !threadPool.empty() || stop; });

                        //trapping the threads in an infinite loop until
                        //task list is empty and there's no more need for work
                        if(stop && threadPool.empty())
                            return;
                        
                        //Grab the next order off the queue
                        task = std::move(OrderList.front());
                        OrderList.pop();
                    }
                }
            }

        }
        ~Market()
        {
            //Locking queue to stop threads
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;

            cv.notify_all();

            //join all threads to properly make them stop
            for(auto& thread : threadPool)
            {
                thread.join();
            }
        }
        void enqueue(std::function<void()> task)
        {
            std::unique_lock<std::mutex> lock(mtx);
            OrderList.emplace(std::move(task));

            //notify the next thread that work is ready
            cv.notify_one();
        }
        void AddOrder(TraderInfo info)
        {
            if(info.type == OrderType::BUY)
            {
                std::unique_lock<std::mutex> lock(mtx);
                BuyList.emplace_back(info);
            }
            else
            {
                std::unique_lock<std::mutex> lock(mtx);
                SellList.emplace_back(info);
            }
        }
    private: 
    std::vector<TraderInfo> BuyList;
    std::vector<TraderInfo> SellList;

    std::vector<std::thread> threadPool;
    std::queue<std::function<void()>> OrderList;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
};