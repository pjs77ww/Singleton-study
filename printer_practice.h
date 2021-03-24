#include <windows.h>
#include <string>
#include <iostream>
#include <queue>
#include <utility>
#include <thread>
using namespace std;

#define Thread __declspec( thread )

class PrinterSystem {
public:
    static PrinterSystem& instance() {
        if (instance_ == nullptr) {
            instance_ = new PrinterSystem();
        }

        return *instance_;
    }
    void GetOrder(int orderNum, int orderCount, string contents) {
        if (orderNum == 1) {
            Order.push(make_pair(orderCount, contents));
            if (!WorkingFlag) {
                DoWork();
            };
        }
        else if(orderNum == 2){
            PrintNowOrder();
        }
        
    }

    void DoWork() {
        WorkingFlag = 1;
        while (Order.size()) {
            PrintWork(Order.front().second);
            Order.pop();
        }
        WorkingFlag = 0;
    }

    void PrintWork(string contents) {
        Sleep(5000);
        cout << "Result is: " << contents << endl;
    }

    void PrintNowOrder() {
        cout << Order.front().first << ". " << Order.front().second << endl;
    }


private:
    PrinterSystem() {}
    static PrinterSystem* instance_;
    static queue <pair<int, string>> Order;
    static bool WorkingFlag;

};