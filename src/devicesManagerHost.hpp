#ifndef DEVICESMANAGER_HOST_HPP
#define DEVICESMANAGER_HOST_HPP

#define WIN32_LEAN_AND_MEAN 1 // Don't include Winsock v1
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <bluetoothapis.h>
#include <WinSock2.h>
#include <ws2bth.h>

// Link with Bluetooth lib
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "Ws2_32.lib")

// normal includes
#include <vector>
#include <iostream>


class DevicesManagerHost {
    private:
    public:
        int startAccept() {
            static LP
            return 0;
        }
};

#endif