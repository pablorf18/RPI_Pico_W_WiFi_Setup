#ifndef WIFISCANNER_WIFI_SCANNER
#define WIFISCANNER_WIFI_SCANNER

/**
 * @file WiFiScanner.h
 * @brief Class to scan and return the available wifi networks
**/

#include <set>
#include "NetworkInfo.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

class WiFiScanner 
{
public:

    WiFiScanner();

    ~WiFiScanner();

    /**
     * Search for available wifi networks
    */
    std::set<NetworkInfo> ScanNetworks();

private:
    //connection timeout is 10s
    uint32_t connection_timeout_ = 10000;

    bool finished_ = false;

    std::set<NetworkInfo> scanned_networks_;

    /**
     * static callback to get the result of the scan
    */
    static int ScanResult(void* env, const cyw43_ev_scan_result_t* result);
};
#endif // WIFISCANNER_WIFI_SCANNER