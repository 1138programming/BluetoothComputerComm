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

#define DEFAULT_BUFLEN 512
//"00014b95-a8f2-43c8-a472-01e65406084f"
//00001101-0000-1000-8000-00805f9b34fb
//DEFINE_GUID(MY_GUID, 0x00014b95, 0xa8f2, 0x43c8, 0xa4, 0x72, 0x01, 0xe6, 0x54, 0x06, 0x08, 0x4f);
static const GUID MY_GUID = {0x00001101, 0x0000, 0x1000, {0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb}};

class DevicesManagerHostTwo {
    private:
    // vars
        SOCKET listener;
        SOCKET connection;
        BLUETOOTH_ADDRESS externalAddress;
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

        //useful functions
        void printBTNameHex(BTH_ADDR addr) {
            std::cout << std::hex;
            BYTE* nameInBytes = (BYTE*)&(addr);
            for (int i = 0; i < 5; i++) {
                std::cout << (int)(nameInBytes[i]) << ":";
            }
            std::cout << (int)(nameInBytes[5]) << std::endl;
            std::cout << std::dec;
        }
        void checkSuccess() {

        }
        int startAccept() {
            // initialize a windows socket in bluetooth mode
            listener = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

            // create a BluetoothAddress to add to the SOCKADDR_BTH (THIS MAY BE WRONG)
            // please make sure to note that the bytes are little-endian (ask Jonathan)
            //BLUETOOTH_ADDRESS addr;
                // addr.rgBytes[0] = 0x9C;
                // addr.rgBytes[1] = 0x90;
                // addr.rgBytes[2] = 0x90;
                // addr.rgBytes[3] = 0X8A;
                // addr.rgBytes[4] = 0x4F;
                // addr.rgBytes[5] = 0x14;
                // addr.ullLong = 280;

            //14:4F:8A:90:90:9C

            // create a bluetooth socket
            SOCKADDR_BTH listenerAddr;
            memset(&listenerAddr, 0, sizeof(listenerAddr));
                listenerAddr.addressFamily = AF_BTH;
                // if 0 = random port
                // setting this to 0 = work, but not great for what we want to do.
                listenerAddr.btAddr = 0;
                listenerAddr.port = BT_PORT_ANY;
                // p sure this will be ignored lol
                listenerAddr.serviceClassId = MY_GUID;

            std::cout << "ckpt0" << std::endl;
            
            // bind to the Bluetooth Socket
            // success checks if there is an error value...
            int success = bind(listener, reinterpret_cast<sockaddr*>(&listenerAddr), sizeof(listenerAddr));
            if (success != 0) {
                return WSAGetLastError();
            }


            std::cout << "ckpt-405" << std::endl;

            // get socket data
            SOCKADDR_BTH socketName = {0};
            int socketNameLen = sizeof(socketName);
            if (getsockname(listener, reinterpret_cast<sockaddr*>(&socketName), &socketNameLen) != 0) {
                std::cerr << "no name lol" << std::endl;
                return WSAGetLastError();
            }
            
            // print name in hex
            printBTNameHex(socketName.btAddr);
            std::cout << "port: " << socketName.port << std::endl;
            
            // all "ckpt" std::couts are just for debugging
            std::cout << "ckpt1" << std::endl;

            // set listener up for connection(?)
            success = listen(listener, 8);
            if (success != 0) {
                return WSAGetLastError();
            }
            
            std::cout << "ckpt2" << std::endl;



            int addrSize = sizeof(externalAddress);
            // blocking function lol- accepts connection, but doesn't get any data about the devices
            connection = accept(listener, nullptr, nullptr);
            if (connection != 0) {
                return WSAGetLastError();
            }

            std::cout << "ckpt3" << std::endl;

            // print address if successfully connected
            // for (int i = 0; i < 14; i++) {
            //     std::cout << externalAddress.sa_data[i];
            // }
            // std::cout << std::endl;
            return 0;
        }
};

#endif