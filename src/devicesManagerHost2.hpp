#ifndef DEVICESMANAGER_HOST2_HPP
#define DEVICESMANAGER_HOST2_HPP

#define WIN32_LEAN_AND_MEAN 1 // Don't include Winsock v1
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <bluetoothapis.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <synchapi.h>

// Link with Bluetooth lib
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "Ws2_32.lib")

// normal includes
#include <vector>
#include <iostream>
#include <stdio.h>


class DevicesManagerHostTwo {
    private:
    // vars
        SOCKET listener;
        SOCKET connection;
        sockaddr externalAddress;
    // consts
    public:
        static int handler() {
            std::cout << "first try 😎" << std::endl;
            return 0;
        }
        int initAll() {
            // Start Winsock on Windows
            WSADATA wsaData;
            return WSAStartup(MAKEWORD(2, 2), &wsaData); // MAKEWORD(2, 2) for Winsock 2.2
        }
        int cleanup() {
            return WSACleanup();
        }
        int startAccept() {

            listener = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

            BLUETOOTH_ADDRESS addr;
                addr.rgBytes[0] = 0xb9;
                addr.rgBytes[1] = 0x10;
                addr.rgBytes[2] = 0x56;
                addr.rgBytes[3] = 0x21;
                addr.rgBytes[4] = 0x66;
                addr.rgBytes[5] = 0x90;

            SOCKADDR_BTH listenerAddr;
                listenerAddr.addressFamily = AF_BTH;
                listenerAddr.btAddr = 0;
                listenerAddr.port = 20;
            
            int success = bind(listener, (sockaddr*)&listenerAddr, sizeof(listenerAddr));
            if (success == SOCKET_ERROR) {
                return WSAGetLastError();
            }

            std::cout << "ckpt1" << std::endl;

            success = listen(listener, SOMAXCONN);
            if (success != 0) {
                return WSAGetLastError();
            }
            
            std::cout << "ckpt2" << std::endl;

            int addrSize = sizeof(externalAddress);
            connection = accept(listener, nullptr, nullptr);
            if (connection == INVALID_SOCKET) {
                return WSAGetLastError();
            }

            std::cout << "ckpt3" << std::endl;

            // print address if successfully connected
            for (int i = 0; i < 14; i++) {
                std::cout << externalAddress.sa_data[i];
            }
            std::cout << std::endl;
            return 0;
        }
};

#endif