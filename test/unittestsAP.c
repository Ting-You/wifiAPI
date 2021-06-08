#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "wifi.h"
#include "utils.h"

/**
 * \brief WiFi AP mode open unittest
 * 
 * \return <0 is error, =0 is success
 * 
*/
int unittest_wifiOpenAP()
{
    /* test ap open */
    WifiAPArgs input;
    WifiAPArgs output;
    /* init test case */
    memset(&input, 0, sizeof(input));
    memset(&output, 0, sizeof(output));
    memcpy(input.ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(input.psk, "lucker123", strlen("lucker123") + 1);
    memcpy(input.interface, "wlan0", strlen("wlan0") + 1);
    input.wpa = 2;
    input.hw_mode = 'a';
    input.channel = 157;
    input.vendorCode = 0x11223301;
    memcpy(input.auth, "WPA-PSK", strlen("WPA-PSK") + 1);
    memcpy(input.serverAddress, "192.168.3.1", strlen("192.168.3.1") + 1);
    input.dhcpEnable = 1;
    wifiOpenAP(&input);

    /* Check configuration file */

    wifiUtilsGetAPConfig(&output);

    if (strncmp(input.ssid, output.ssid, sizeof(input.ssid)) != 0)
    {
        LIBWIFI_ERROR_PRINT("ssid not match...\r\n");
    }
    if (strncmp(input.psk, output.psk, sizeof(input.psk)) != 0)
    {
        LIBWIFI_ERROR_PRINT("psk not match...\r\n");
    }
    if (strncmp(input.interface, output.interface, sizeof(input.interface)) != 0)
    {
        LIBWIFI_ERROR_PRINT("interface not match...\r\n");
    }
    if (input.wpa != output.wpa)
    {
        LIBWIFI_ERROR_PRINT("wpa not match...\r\n");
    }
    if (input.hw_mode != output.hw_mode)
    {
        LIBWIFI_ERROR_PRINT("hw mode not match...\r\n");
    }
    if (input.channel != output.channel)
    {
        LIBWIFI_ERROR_PRINT("channel not match...\r\n");
    }
    if (strncmp(input.auth, output.auth, sizeof(input.auth)) != 0)
    {
        LIBWIFI_ERROR_PRINT("auth not match...\r\n");
    }

    return 0;
}
int unittest_wifiCloseAP()
{
    wifiCloseAP();
    return 0;
}

int unittest_wifiRestartAP()
{
    WifiAPArgs args;
    wifiUtilsAPInit(&args);
    wifiRestartAP(&args);
    return 0;
}

int main()
{
    ASSERT((unittest_wifiOpenAP() == 0),"unittest_wifiOpenAP fail.\r\n");
    // ASSERT(unittest_wifiCloseAP() == 0,"unittest_wifiCloseAP fail.\r\n");
    // ASSERT(unittest_wifiRestartAP() == 0,"unittest_wifiRestartAP fail.\r\n");

    return 0;
}
