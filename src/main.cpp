#include "devicesManagerClient.hpp"
#include "devicesManagerHost.hpp"

int main() {
    DevicesManagerHost manager;
    manager.initAll();
    int b = manager.startAccept();
    int a;
    std::cout << b;
    std::cin >> a;
    return 0;
}