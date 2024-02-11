#ifndef DEVICESMANAGER_CLIENT_HPP
#define DEVICESMANAGER_CLIENT_HPP

// BT includes
#define WIN32_LEAN_AND_MEAN 1 // Don't include Winsock v1
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
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

class DevicesManagerClient {
    private:
        std::vector<BLUETOOTH_DEVICE_INFO> devices;
        std::vector<SOCKET> connections;
    public:
        /**
         * @return SOCKET ERORR: -1 = NO DEVICES CONNECTED
         * @return  &&  SUCCESS: 0
        */
        int initAll() {
            // Start Winsock on Windows
            WSADATA wsaData;
            return WSAStartup(MAKEWORD(2, 2), &wsaData); // MAKEWORD(2, 2) for Winsock 2.2
        }
        int clean() {
            WSACleanup();
        }
        int close() {
            for (int i = 0; i < this->connections.size(); i++) {
                closesocket(this->connections.at(i));
            }
            return 0;
        }
        int initCurrentlyRemembered() {
            BLUETOOTH_DEVICE_SEARCH_PARAMS searchCriteria;
            // init all necessary values for search (only return paired devices)
                searchCriteria.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
                searchCriteria.fReturnAuthenticated = false;
                searchCriteria.fReturnRemembered = true;
                searchCriteria.fReturnUnknown = false;
                searchCriteria.fReturnConnected = false;
                searchCriteria.fIssueInquiry = false;
                // timeout- infinite time prob.
                searchCriteria.cTimeoutMultiplier = 0;

            // set up device struct
            BLUETOOTH_DEVICE_INFO deviceInfo;
                deviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
            
            // get first device (to check if there are any to pair)
            HBLUETOOTH_DEVICE_FIND foundDevice = BluetoothFindFirstDevice(&searchCriteria, &deviceInfo);
            if (!foundDevice) {
                return SOCKET_ERROR;
            }

            // loop through all valid devices and add them to internal vector
            do {
                this->devices.push_back(deviceInfo);
            } while (BluetoothFindNextDevice(foundDevice, &deviceInfo));
            return 0;
        }

        /**
         * Returns:
         * -40452: None initialized
         * -1: socket init failed
         * 0: successful
        */
        int connectToFirstInitialized() {
            // check if none initialized
            if (this->devices.size() == 0) {
                return -40452;
            }

            //just get first one for now
            BLUETOOTH_DEVICE_INFO info = this->devices.at(0);

            SOCKET socketft; // socket file type
            // assumes the socket is using bluetooth
            socketft = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
            if (socketft == INVALID_SOCKET) {
                return INVALID_SOCKET;
            }

            int nonBlockRet = 1;
            ULONG mode = 1;
            nonBlockRet = ioctlsocket(socketft, FIONBIO, &mode);

            if (nonBlockRet == SOCKET_ERROR) {
                return -3;
            }
            

            // set up socket address
            SOCKADDR_BTH addr;
                addr.addressFamily = AF_BTH;
                addr.btAddr = info.Address.ullLong;
                addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
                addr.port = 0;

            // hoping this works :prayer:
            connect(socketft, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
            this->connections.push_back(socketft);

            // maybe not :skull:
            return 0;
        }

        int sendDataToFirstConnected(std::string* data) {
            // check to make sure there is a connection
            if (this->connections.size() == 0) {
                return -40452;
            }
            SOCKET socketft = this->connections.at(0);
            // static casting bc function expecting int && example code does it :shrug:
            return send(socketft, data->c_str(), static_cast<int>(data->size()), MSG_BCAST);
        }

        std::vector<BYTE> getDataFromFirstConnected() {
            if (this->connections.size() == 0) {
                std::vector<BYTE> nullret;
                nullret.push_back(-40452);
                return nullret;
            }

            SOCKET soc = this->connections.at(0);
            
            const int bufLen = 1024;
            BYTE buf[bufLen];

            int ret = recv(soc, (char*)buf, (bufLen/2) - 1, MSG_BCAST);
            if (ret != SOCKET_ERROR) {
                ((char*)buf)[bufLen/2] = '\0';
            }
            // get return vector and return
            std::vector<BYTE> returnVal;
            returnVal.reserve(bufLen);
            for (int i = 0; i < bufLen; i++) {
                returnVal.push_back(buf[i]);
            }
            return returnVal;
        }

        std::string getDataFromFirstConnectedAsString() {
            std::vector<BYTE> byteData = getDataFromFirstConnected();

            //convert to byte[]
            BYTE* byteArr = (BYTE*)malloc(sizeof(BYTE) * byteData.size());
            for (int i = 0; i < byteData.size(); i++) {
                byteArr[i] = byteData.at(i);
            }
            std::string returnVal((char*)byteArr);
            // free malloc()ed arr
            free(byteArr);
            return returnVal;
        }

        void printDevices() {
            std::cout << "currently paried:" <<  std::endl;
            for (int i = 0; i < this->devices.size(); i++) {
                BLUETOOTH_DEVICE_INFO info = this->devices.at(i);
                for (int i = 0; i < 31; i++) {
                    std::cout << (char)info.szName[i];
                }
                std::cout << std::endl;
            }
        }
        void printAddress() {
            std::cout << "mac addresses:" << std::endl;
            for (int i = 0; i < this->devices.size(); i++) {
                BLUETOOTH_DEVICE_INFO info = this->devices.at(i);
                for (int i = 0; i < 6; i++) {
                    std::cout << (info.Address.rgBytes[i] & 0xFF) << "-";
                }
                std::cout << std::endl;
            }
        }
};

#endif