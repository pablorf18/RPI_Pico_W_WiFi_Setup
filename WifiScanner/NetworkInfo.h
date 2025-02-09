#ifndef WIFISCANNER_NETWORK_INFO
#define WIFISCANNER_NETWORK_INFO

/**
 * @file NetworkInfo.h
 * @brief The information about a scanned network
**/

#include <string>
#include <cstdint>

struct NetworkInfo 
{
    // the name of the network
    std::string ssid;
    // Received Signal Strength Indicator
    int rssi = 0;
    // channel of the wifi network
    int channel = 0;
    // the mac (or bssid) of the wifi network
    std::string mac;
    //authentication mode
    uint8_t auth_mode = 0;

    //overload the equal operator
    bool operator=(const NetworkInfo & ntw_info) const
    {
        return ntw_info.ssid == ssid;
    }

    //overload the < (less than) operator
    bool operator<(const NetworkInfo & ntw_info) const
    {
        return ssid < ntw_info.ssid;
    }
};

#endif // WIFISCANNER_NETWORK_INFO