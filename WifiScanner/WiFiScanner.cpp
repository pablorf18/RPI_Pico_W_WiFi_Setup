#include "WiFiScanner.h"

WiFiScanner::WiFiScanner()
{
}

WiFiScanner::~WiFiScanner()
{
}

std::set<NetworkInfo> WiFiScanner::ScanNetworks()
{
    scanned_networks_.clear();

    //start time of the scan operation
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    
    bool scan_in_progress = false;
    while(true) 
    {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if((current_time - start_time) < connection_timeout_)  
        {
            if (!scan_in_progress) 
            {
                cyw43_wifi_scan_options_t scan_options = {0};
                int err = cyw43_wifi_scan(&cyw43_state, &scan_options, this, ScanResult);
                if (err == 0)
                {
                    //scan is in progress
                    printf("WiFiScanner::ScanNetworks: Performing wifi scan\n");
                    scan_in_progress = true;
                }
                else 
                {
                    //scan fails
                    printf("WiFiScanner::ScanNetworks: Failed to start scan: %d\n", err);
                }
            } 
            else if (!cyw43_wifi_scan_active(&cyw43_state)) 
            {
                printf("WiFiScanner::ScanNetworks: scan finished\n");
                break;
            }
        }
        else
        {
            printf("WiFiScanner::ScanNetworks: timeout reached\n");
            break;
        }
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
    }

    finished_ = true;

    return scanned_networks_;
}

int WiFiScanner::ScanResult(void* env, const cyw43_ev_scan_result_t* result)
{
    auto* scanner = static_cast<WiFiScanner*>(env);
    if (result) 
    {
        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
            result->ssid, result->rssi, result->channel,
            result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
            result->auth_mode);
        NetworkInfo ntw_info;
        ntw_info.ssid = std::string(reinterpret_cast<const char*>(result->ssid), result->ssid_len);
        ntw_info.rssi = result->rssi;
        ntw_info.channel = result->channel;
        ntw_info.mac = std::string(reinterpret_cast<const char*>(result->bssid), sizeof(result->bssid));
        ntw_info.auth_mode = result->auth_mode;

        if(scanner != nullptr)
        {
            if(!scanner->finished_)
            {
                scanner->scanned_networks_.insert(ntw_info);
                printf("Added network with ssid:%s \n", ntw_info.ssid.c_str());
            }
            else
            {
                printf("Scanner already finished\n");
            }
        }
        else
        {
            printf("Scanner is nullptr\n");
        }
    }

    return 0;
}