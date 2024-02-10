#include "devicesManager.hpp"

int main() {
    DevicesManager manager;
    manager.initCurrentlyRemembered();
    manager.printDevices();
    
    std::string a;
    std::cin >> a;
    return 0;
}