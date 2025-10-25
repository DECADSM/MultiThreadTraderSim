#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

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
    TraderInfo(int id, double price, OrderType t, int q) : traderID(id), _price(price), type(t), quantity(q) { } 
    int traderID;
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
            if(amt > std::thread::hardware_concurrency())
            {
                std::cout << "Asked for too many threads. Current hardware can support about " << std::thread::hardware_concurrency() << ". Please check your hardware for proper thread counts.";
                return;
            }
            num_of_threads = amt;


        }
        void Buy(TraderInfo info);
        void Buy(int id, double price, OrderType type, int amount);

        void Sell(TraderInfo info); 
        void Sell(int id, double price, OrderType type, int amount);
    
    private: 
    std::vector<TraderInfo> BuyList;
    std::vector<TraderInfo> SellList;

    std::vector<std::thread> threadPool;
    int num_of_threads;
    std::mutex mtx;
    bool stop = false;
};