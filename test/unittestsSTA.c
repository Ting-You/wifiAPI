#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "wifi.h"
#include "utils.h"

/**
 * \brief WiFi STA mode open unittest
 * 
 * \return <0 is error, =0 is success
 * 
*/
int unittest_wifiOpenSTA()
{
    /* test sta open */
    WifiSTAArgs input;
    WifiSTAArgs output;
    /* init test case */
    memset(&input, 0, sizeof(WifiSTAArgs));
    memset(&output, 0, sizeof(WifiSTAArgs));
    memcpy(input.ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(input.psk, "lucker123", strlen("lucker123") + 1);
    memcpy(input.auth, "WPA-PSK", strlen("WPA-PSK") + 1);

    wifiOpenSTA(&input);

    /* Check configuration file */
    wifiUtilsGetSTAConfig(&output);
    if (strncmp(input.ssid, output.ssid, sizeof(input.ssid)) != 0)
    {
        LIBWIFI_ERROR_PRINT("ssid not match...\r\n");
    }
    else
    {
        LIBWIFI_INFO_PRINT("ssid OK!!!\r\n");
    }
    if (strncmp(input.psk, output.psk, sizeof(input.psk)) != 0)
    {
        LIBWIFI_ERROR_PRINT("psk not match...\r\n");
    }
    else
    {
        LIBWIFI_INFO_PRINT("PSK OK!!!\r\n");
    }
    if (strncmp(input.auth, output.auth, sizeof(input.auth)) != 0)
    {
        LIBWIFI_ERROR_PRINT("auth not match...\r\n");
    }
    else
    {
        LIBWIFI_INFO_PRINT("auth OK!!!\r\n");
    }
    return 0;
}

int unittest_wifiCloseSTA()
{
    wifiCloseSTA();
    return 0;
}

int unittest_wifiRestartSTA()
{
    WifiSTAArgs args;
    wifiUtilsSTAInit(&args);
    wifiRestartSTA(&args);
    return 0;
}

int main()
{
     ASSERT((unittest_wifiOpenSTA() == 0), "unittest_wifiOpenSTA fail.\r\n");
    // ASSERT(unittest_wifiCloseSTA() == 0, "unittest_wifiCloseSTA fail.\r\n");
    // ASSERT(unittest_wifiRestartSTA() == 0, "unittsest_wifiRestartSTA fail.\r\n");

     WifiScanResults args;

    wifiGetAPList(&args);

    wifiPrintAPInformation(wifiGetAPInformation(&args, "IoT-lucker"));

    return 0;
}