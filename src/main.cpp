#include "devicesManagerClient.hpp"

int main() {
    DevicesManagerClient manager;
    manager.initAll();
    int d = manager.initCurrentlyRemembered();
    manager.printDevices();
    int a = manager.connectToFirstInitialized();
    int b = manager.sendDataToFirstConnected(&std::string("HELLO!"));
    
    std::cout << d << " && " << a << " && " << b << std::endl;

    std::string c;
    std::cin >> c;
    manager.close();
    return 0;
}