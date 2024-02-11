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
#include <MSWSock.h>
#include <bluetoothapis.h>
#include <WinSock2.h>
#include <ws2bth.h>
#include <synchapi.h>

// Link with Bluetooth lib
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "Ws2_32.lib")

// normal includes
#include <vector>
#include <iostream>


class DevicesManagerHost {
    private:
    // vars
        SOCKET sock;
        std::vector<BYTE> acceptExBuf;
        SOCKET clientSocket;
        DWORD bytesRecvd = 0;
    // consts
        // according to docs, this is the minimum size (https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex)
        const DWORD addrSize = sizeof(sockaddr_storage) + 16; 
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
            // idk- creating client && server?
            acceptExBuf.reserve(addrSize * 4);

            // init socket- may neeed to change values
            sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

            // docs tell me to (https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex)
            // yea the socket needs to be listening for this...
            listen(sock, SOMAXCONN);

            std::cout << "ckpt1" << std::endl;

            static LPFN_ACCEPTEX acceptExPtr = nullptr;
            int success;

            std::cout << "ckpt2" << std::endl;

            if (!acceptExPtr) {
                GUID guid = WSAID_ACCEPTEX;
                DWORD numBytes = 0;

                std::cout << "ckpt3" << std::endl;
                success = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &acceptExPtr, sizeof(acceptExPtr), &numBytes, nullptr, nullptr);
                if (success != 0) {
                    return WSAGetLastError();
                }

                std::cout << "ckpt4" << std::endl;
            }

            OVERLAPPED overlap;
                // a handle is required so we just make one that tells it to do nothing lol
                overlap.hEvent = CreateEventA(nullptr, false, false, nullptr);
            
            //success = acceptExPtr(sock, clientSocket, acceptExBuf.data(), 0, addrSize, addrSize, &bytesRecvd, &overlap);
            if (success == false) {
                return WSAGetLastError();
            }
            return 0;
        }
};

#endif