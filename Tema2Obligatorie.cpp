/*

Temă obligatorie: să se creeze o aplicație  (preferabil consolă) 
în C sau C++ care să afișeze toate dispozitivele conectate la magistrala 
USB a mașinii pe care rulați codul.

*/
#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>

// Link with the setupapi library to use its functions
#pragma comment(lib, "setupapi.lib")

int main() {

    // Get a list of all connected devices of the USB class
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, nullptr, nullptr, DIGCF_PRESENT);
    // If no devices are found or there is an error, print an error message and exit
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "We could not get info about any USB Devices" << std::endl;
        return 1;
    }
    //Store information about each device
    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(devInfoData);
    DWORD index = 0;

    // Iterate through all USB devices found in the device information list
    while (SetupDiEnumDeviceInfo(hDevInfo, index, &devInfoData)) {
        char deviceName[256] = "";
        // Try getting Friendly Name first
        if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr, (PBYTE)deviceName, sizeof(deviceName), nullptr)) {
            // If Friendly Name is not available, get Device Description instead
            if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_DEVICEDESC, nullptr, (PBYTE)deviceName, sizeof(deviceName), nullptr)) {
                // If neither property is available, set a default message
                strcpy_s(deviceName, "Unknown USB Device");
            }
        }
        // Output the name of the USB device
        std::cout << "USB Device: " << deviceName << std::endl;
        index++;
    }
    // Clean up by destroying the device information list to release memory
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return 0;
}
