#include "devicesManagerClient.hpp"
#include "devicesManagerHost.hpp"
#include "devicesManagerHost2.hpp"

int main() {
    DevicesManagerHostTwo manager;
    int c = manager.initAll();
    int b = manager.startAccept();
    int a;
    std::cout << c << " + " <<  b << std::endl;
    manager.readFromConnectionAndPrint();
    manager.cleanup();
    std::cin >> a;
    return 0;
}