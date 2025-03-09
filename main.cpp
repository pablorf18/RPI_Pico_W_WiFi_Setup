#include "lwip/tcp.h"
#include "lwip/ip4_addr.h"
#include <sstream>
#include <cctype>

#include "lwip/apps/fs.h"

#include "dhcpserver.h"

#include "CGI.h"
#include "SSI.h"

char * ssid = NULL;
char * password = NULL;

bool wifi_connected = false;

bool ConnectToWifi(const char * user_ssid, const char * user_password)
{
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(user_ssid, user_password, CYW43_AUTH_WPA2_AES_PSK, 10000)) 
    {
        printf("Failed to connect to Wi-Fi\n");
        return false;
    }
    printf("Connected to Wi-Fi\n");
    return true;
}

const char* ConnectWiFiCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) 
{   
    if(operation_in_progress)
    {
        printf("Operation in progress\n");
        return "/index.shtml";
    }

    operation_in_progress = true;
    printf("WiFi Connection Request:\n");

    char * user_ssid = NULL;
    char * user_password = NULL;

    for (int i = 0; i < iNumParams; i++) 
    {
        if (strcmp(pcParam[i], "ssid") == 0) 
        {
            user_ssid = UrlDecode(pcValue[i]);
        } 
        else if (strcmp(pcParam[i], "password") == 0) 
        {
            user_password = UrlDecode(pcValue[i]);
        }
    }

    const char* returnPage = "/error.json";
    
    if (user_ssid && user_password) 
    {
        cyw43_arch_enable_sta_mode();
        printf("Attempting to connect to SSID: %s, %s\n", user_ssid, user_password);

        bool connected = ConnectToWifi(user_ssid, user_password);
        cyw43_wifi_leave(&cyw43_state, CYW43_ITF_STA);

        if(connected)
        {
            printf("Connection successful\n");
            returnPage = "/success.json";
            wifi_connected = true;

            ssid = (char*)malloc(strlen(user_ssid) + 1);
            strcpy(ssid, user_ssid);
            password = (char*)malloc(strlen(user_password) + 1);
            strcpy(password, user_password);
        }
    } 
    else
    {
        printf("Invalid parameters\n");
    } 

    if(user_ssid)
    {
        free(user_ssid);
    }
    if(user_password)
    {
        free(user_password);
    }

    printf("returnPage is %s\n", returnPage);
    operation_in_progress = false;

    return returnPage;
}

int main() 
{
    stdio_init_all();
    // Wait for serial console to connect
    sleep_ms(3000); 

    printf("RPI PICO W AP DEMO\n");

    // Initialize CYW43 driver
    if (cyw43_arch_init()) 
    {
        printf("Failed to initialize CYW43 driver\n");
        return 1;
    }

    // Set up access point
    cyw43_arch_enable_ap_mode(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    cyw43_wifi_ap_set_channel(&cyw43_state, 1); // Set channel
    cyw43_state.ap_tx_power = 20; // Set transmit power to 20 dBm

    bool ap_mode_enabled = true; 
    // Print AP info
    printf("Access Point active. SSID: %s, Password: %s\n", WIFI_SSID, WIFI_PASSWORD);

    httpd_init();
    SSIInit();
    CGIInit(ConnectWiFiCGIHandler);

    // Set new IP configuration
    ip4_addr_t ip, netmask, gateway;
    IP4_ADDR(&ip, 192, 168, 1, 1);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gateway, 192, 168, 1, 1);

    // Apply the new configuration
    netif_set_addr(netif_default, &ip, &netmask, &gateway);

    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &ip, &netmask);

    // Main loop
    while(1)
    {
        // Handle any pending network events
        cyw43_arch_poll();
        sleep_ms(10);
        if(wifi_connected)
        {
            if(ap_mode_enabled)
            {
                int i = 0;
                while(i < 100)
                {
                    cyw43_arch_poll();
                    sleep_ms(100);
                    i++;
                }
                printf("disabling ap mode\n");
                cyw43_arch_disable_ap_mode();
                ap_mode_enabled = false;
                wifi_connected = false;
                int retries = 3;
                while(!wifi_connected && retries > 0)
                {
                    wifi_connected = ConnectToWifi(ssid, password);
                    retries--;
                }
                ip4_addr_t ip = cyw43_state.netif->ip_addr;
                printf("RPI IP Address: %s\n", ip4addr_ntoa(&ip));
            }
        }
    }

    printf("End of program\n");

    cyw43_arch_deinit();

    if(ssid)
    {   
        free(ssid);
    }
    if(password)
    {
        free(password);
    }

    return 0;
}
