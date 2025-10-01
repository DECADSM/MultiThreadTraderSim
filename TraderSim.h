#include <string>
#include <iostream>

class Trader{
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

struct TradeInfo {
    public:
    TradeInfo(int id, double price) : traderID(id), _price(price) { } 
    int traderID;
    double _price;
    void Print()
    {
        std::cout << "Trader " << traderID << " made a trade for this price: " << _price << std::endl;
    }
};