#define WIN32_LEAN_AND_MEAN 1 // Don't include Winsock v1
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#include <ws2bth.h>
#include <WS2tcpip.h>

#include <iostream>

int main() {
    int a;
    std::cin >> a;
    return 0;
}