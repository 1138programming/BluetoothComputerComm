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

// Link with Bluetooth lib
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "Ws2_32.lib")

// normal includes
#include <vector>
#include <iostream>
#include <coroutine> // IWYU pragma: keep


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
        int startAccept() {
            // idk
            acceptExBuf.reserve(addrSize);
            // docs tell me to (https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex)
            listen(sock, SOMAXCONN);

            static LPFN_ACCEPTEX acceptExPtr = nullptr;
            int success;

            if (!acceptExPtr) {
                GUID guid = WSAID_ACCEPTEX;
                DWORD numBytes = 0;

                success = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &acceptExPtr, sizeof(acceptExPtr), &numBytes, nullptr, nullptr);
                if (success != 0) {
                    return -1;
                }
            }
            success = acceptExPtr(sock, clientSocket, acceptExBuf.data(), 0, addrSize, addrSize, &bytesRecvd, );
            if (success != 0) {
                return -2;
            }
            return 0;
        }
};

#endif