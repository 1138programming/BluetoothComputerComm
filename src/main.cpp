#include "devicesManager.hpp"

int main() {
    DevicesManager manager;
    manager.initCurrentlyRemembered();
    manager.printDevices();
    
    int a;
    std::cin >> a;
    return 0;
}