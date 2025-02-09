#ifndef WEB_HANDLERS_CGI_H
#define WEB_HANDLERS_CGI_H

/**
 * @file CGI.h
 * @brief Common Gateway Interface, used to handle HTTP requests sent by the client
**/

#include "lwip/apps/httpd.h"
#include <stdio.h>
#include <vector>

#include "WiFiScanner.h"

static std::vector<std::string> wifi_networks;

static bool operation_in_progress = false;

/**
 * @brief Get the list of available WiFi networks
 * @return The list of available WiFi networks
*/
std::vector<std::string> GetWiFiNetworks();

/**
 * @brief Decode a URL-encoded string
 * @param src The URL-encoded string
 * @return The decoded string
*/
char* UrlDecode(const char* src);

/**
 * @brief Scan for available WiFi networks
*/
void ScanWiFi();

/**
 * @brief Handle the WiFi scan request
 * @param iIndex The index of the CGI handler
 * @param iNumParams The number of parameters
 * @param pcParam The parameter names
 * @param pcValue The parameter values
 * @return The page to redirect to
*/
const char* ScanWiFiCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/**
 * @brief Initialize the CGI handlers
 * @param ConnectCallback The callback function to handle the connection request
*/
void CGIInit(const char* (*ConnectCallback)(int, int, char*[], char*[]));

#endif // WEB_HANDLERS_CGI_H