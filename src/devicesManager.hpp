#ifndef DEVICESMANAGER_HPP
#define DEVICESMANAGER_HPP

// BT includes
#define WIN32_LEAN_AND_MEAN 1 // Don't include Winsock v1
#include <windows.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <bluetoothapis.h>
#include <WinSock2.h>
#include <ws2bth.h>

// Link with Bluetooth lib
#pragma comment(lib, "Bthprops.lib")

// normal includes
#include <vector>
#include <iostream>

class DevicesManager {
    private:
        std::vector<BLUETOOTH_DEVICE_INFO> devices;
    public:
        /**
         * @return SOCKET ERORR: -1 = NO DEVICES CONNECTED
         * @return  &&  SUCCESS: 0
        */
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
};

#endif