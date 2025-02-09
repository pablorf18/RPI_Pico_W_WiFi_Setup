#include "CGI.h"

std::vector<std::string> GetWiFiNetworks()
{
    return wifi_networks;
}

char* UrlDecode(const char* src) 
{
    size_t src_len = strlen(src);
    char* decoded = (char*)malloc(src_len + 1);
    size_t decoded_len = 0;

    for (size_t i = 0; i < src_len; i++) 
    {
        if (src[i] == '%' && i + 2 < src_len) 
        {
            int hex_val;
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len++] = hex_val;
            i += 2;
        } 
        else if (src[i] == '+') 
        {
            decoded[decoded_len++] = ' ';
        } 
        else 
        {
            decoded[decoded_len++] = src[i];
        }
    }

    decoded[decoded_len] = '\0';
    return decoded;
}

void ScanWiFi() 
{
    wifi_networks.clear();
    WiFiScanner wifi_scanner;
    auto networks = wifi_scanner.ScanNetworks();
    for(const auto & network : networks)
    {
        wifi_networks.push_back(network.ssid);
    }
}

const char* ScanWiFiCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) 
{
    if(operation_in_progress)
    {
        printf("Operation in progress\n");
        return "/index.shtml";;
    }
    
    operation_in_progress = true;
    ScanWiFi();
    
    operation_in_progress = false;
    return "/index.shtml";
}

void CGIInit(const char* (*ConnectCallback)(int, int, char*[], char*[]))
{
    // CGI handlers array
    static const tCGI kCGIHandlers[] = 
    {
        {"/connect", ConnectCallback},
        {"/scan_wifi", ScanWiFiCGIHandler},
    };
    
    http_set_cgi_handlers(kCGIHandlers, LWIP_ARRAYSIZE(kCGIHandlers));
}
