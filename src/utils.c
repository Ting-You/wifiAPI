#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int wifiUtilsGetSTAConfig(WifiSTAArgs *args)
{
    FILE *fp = fopen(WPA_CONFIG_PATH, "r");
    char line[256];
    char key[128];
    char value[128];
    if (!fp)
    {
        LIBWIFI_ERROR_PRINT("Cannot open " WPA_CONFIG_PATH "");
        return -1;
    }
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        sscanf(line, "%[^=\n\r]=%[^=\n\r]", key, value);
        if (strncmp(key, "\tssid", strlen("\tssid")) == 0)
        {
            memset(args->ssid, 0, sizeof(args->ssid));
            /* remove \" characters  */
            memcpy(args->ssid, value + 1, 10);
        }
        if (strncmp(key, "\tpsk", strlen("\tpsk")) == 0)
        {
            memset(args->psk, 0, sizeof(args->psk));
            memcpy(args->psk, value + 1, 9);
        }
        if (strncmp(key, "\tkey_mgmt", strlen("\tkey_mgmt")) == 0)
        {
            memset(args->auth, 0, sizeof(args->auth));
            memcpy(args->auth, value, sizeof(args->auth) - 1);
        }
    }
    fclose(fp);
    return 0;
}
int wifiUtilsGetAPConfig(WifiAPArgs *args)
{
    FILE *fp = fopen(HOSTAP_CONFIG_PATH, "r");
    char line[256];
    char key[128];
    char value[128];
    if (!fp)
    {
        LIBWIFI_ERROR_PRINT("Cannot open "HOSTAP_CONFIG_PATH"\r\n");
        return -1;
    }
    while (fgets(line, sizeof(line), fp) != NULL)
    {

        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        sscanf(line, "%[^=\n\r]=%[^=\n\r]", key, value);

        if (strncmp(key, "ssid", strlen("ssid")) == 0)
        {
            memset(args->ssid, 0, sizeof(args->ssid));
            memcpy(args->ssid, value, (sizeof(args->ssid) - 1));
        }

        if (strncmp(key, "wpa_passphrase", strlen("wpa_passphrase")) == 0)
        {
            memset(args->psk, 0, sizeof(args->psk));
            memcpy(args->psk, value, (sizeof(args->psk) - 1));
        }

        if (strncmp(key, "interface", strlen("interface")) == 0)
        {
            memset(args->interface, 0, sizeof(args->interface));
            memcpy(args->interface, value, (sizeof(args->interface) - 1));
        }

        if (strncmp(key, "wpa", 4) == 0)
        {
            args->wpa = 0;
            args->wpa = atoi(value);
        }

        if (strncmp(key, "hw_mode", strlen("hw_mode")) == 0)
        {
            args->hw_mode = '\0';
            args->hw_mode = value[0];
        }

        if (strncmp(key, "channel", strlen("channel")) == 0)
        {
            args->channel = 0;
            args->channel = atoi(value);
        }

        if (strncmp(key, "vendor_elements", strlen("vendor_elements")) == 0)
        {
            memset(args->vendor_elements, 0, sizeof(args->vendor_elements));
            memcpy(args->vendor_elements, value, (sizeof(args->vendor_elements) - 1));
        }

        if (strncmp(key, "wpa_key_mgmt", strlen("wpa_key_mgmt")) == 0)
        {
            memset(args->auth, 0, sizeof(args->auth));
            memcpy(args->auth, value, (sizeof(args->auth) - 1));
        }
    }
    fclose(fp);
    fp = NULL;
    return 0;
}
int wifiUtilsAPInit(WifiAPArgs *args)
{
    memset(args, 0, sizeof(args));
    memcpy(args->ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(args->psk, "lucker123", strlen("lucker123") + 1);
    memcpy(args->interface, "wlan0", strlen("wlan0") + 1);
    args->wpa = 2;
    args->hw_mode = 'a';
    args->channel = 157;
    args->vendorCode=0x11223301;
    memcpy(args->auth, "WPA-PSK", strlen("WPA-PSK") + 1);
    memcpy(args->serverAddress, "192.168.3.1", strlen("192.168.3.1") + 1);
    args->dhcpEnable = 1;
    return 0;
}
int wifiUtilsSTAInit(WifiSTAArgs *args)
{
    memset(args, 0, sizeof(args));
    memcpy(args->ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(args->psk, "lucker123", strlen("lucker123") + 1);
    memcpy(args->auth, "WPA-PSK", strlen("WPA-PSK") + 1);
    return 0;
}
