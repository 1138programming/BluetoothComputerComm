#include "devicesManager.hpp"

int main() {
    DevicesManager manager;
    int d = manager.initCurrentlyRemembered();
    manager.printDevices();
    int a = manager.connectToFirstInitialized();
    int b = manager.sendDataToFirstConnected(&std::string("HELLO!"));
    
    std::cout << d << " && " << a << " && " << b << std::endl;

    std::string c;
    std::cin >> c;
    return 0;
}