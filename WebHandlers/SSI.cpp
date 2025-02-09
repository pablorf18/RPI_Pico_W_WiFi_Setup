#include "SSI.h"

u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
    printf("SSIHandler called\n");
    size_t printed = 0;
    switch (iIndex) 
    {
        case 0:
        {
            std::string network_list;
            for (const auto& network : GetWiFiNetworks()) 
            {
                network_list += "<option value=\"" + network + "\">" + network + "</option>";
            }
            size_t copied = std::min(network_list.length(), static_cast<size_t>(iInsertLen - 1));
            strncpy(pcInsert, network_list.c_str(), copied);
            pcInsert[copied] = '\0';
            return copied;
        }
    }

    printf("SSIHandler end\n");

    return (u16_t)printed;
}

void SSIInit() 
{
    http_set_ssi_handler(SSIHandler, kSSITags, LWIP_ARRAYSIZE(kSSITags));
}
