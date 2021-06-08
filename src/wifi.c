#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wifi.h"
#include "utils.h"

/*Close AP STA buffer line */
#define LEN 256

int wifiOpenSTA(WifiSTAArgs *args)
{
    FILE *fp = NULL;
    char config[1024] = {0};
    WifiScanResults results;

    ASSERT(args, "Invalid argument\r\n");

    fp = fopen("" WPA_CONFIG_PATH "", "w");
    ASSERT(fp, "Cannot open " WPA_CONFIG_PATH "\r\n");

    /*create wpa_supplicant.conf content*/
    snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
             "ctrl_interface=/var/run/wpa_supplicant\n"
             "update_config=1\n"
             "network={\n");

    ASSERT(strlen(args->ssid), "Invalid SSID:%s\r\n", args->ssid);

    snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
             "\tssid=\"%s\"\n",
             args->ssid);

    /* check security protocol */
    if (strncmp(args->auth, "WPA-PSK", strlen("WPA-PSK")) == 0)
    {
        /*check psk & auth*/
        ASSERT(strlen(args->psk), "Invalid psk:%s\r\n", args->psk);
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "\tpsk=\"%s\"\n"
                 "\tkey_mgmt=%s\n"
                 "\tpairwise=CCMP\n"
                 "\t}\n",
                 args->psk, args->auth);
    }
    /* To be repaired */
    else if (strncmp(args->auth, "WPA-EAP", strlen("WPA-EAP")) == 0)
    {
        ASSERT(strlen(args->psk), "Invalid psk:%s\r\n", args->psk);
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "\tpsk=\"%s\"\n"
                 "\tkey_mgmt=%s\n"
                 "\tpairwise=TKIP CCMP\n"
                 "\t}\n",
                 args->psk, args->auth);
    }
    else
    {
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "\tkey_mgmt=NONE\n"
                 "}\n");
    }

    fprintf(fp, "%s", config);
    fclose(fp);
    fp = NULL;

    system("ifconfig wlan0 up");
    system("wpa_supplicant -B -Dnl80211 -iwlan0 -c " WPA_CONFIG_PATH " -P " WPA_SUPPLICANT_PID_PATH "");
    system("udhcpc -i wlan0 --pidfile " UDHCPC_PID_PATH "");

    /* prevent first time scan empty */
    sleep(1);

    return 0;
}

int wifiOpenAP(WifiAPArgs *args)
{
    FILE *fp = NULL;
    char config[1024] = {0};
    char ifconfig_cmd[32] = {0};
    char IPv4[128] = {0};
    char vendorElement[64] = {0};
    int chooseChannel;
    int chooseWPA = 2;
    char chooseHw = 'a';
    ASSERT(args, "Invalid argument\r\n");

    fp = fopen(HOSTAP_CONFIG_PATH, "w");
    ASSERT(fp, "Cannot open " HOSTAP_CONFIG_PATH "\r\n");

    /* check ssid */
    ASSERT(strlen(args->ssid), "Invalid SSID:%s\r\n", args->ssid);
    snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
             "ssid=%s\n",
             args->ssid);

    /* check interface */
    if (strlen(args->interface) > 0)
    {
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "interface=%s\n", args->interface);
        snprintf(ifconfig_cmd, sizeof(ifconfig_cmd), "ifconfig %s up", args->interface);
    }
    else
    {
        /* default use wlan0 */
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "interface=wlan0\n");
        snprintf(ifconfig_cmd, sizeof(ifconfig_cmd), "ifconfig wlan0 up");
    }

    /* check security protocol */
    if (strncmp(args->auth, "WPA-PSK", strlen("WPA-PSK")) == 0)
    {
        /*check auth*/
        if (args->wpa > 3)
        {
            LIBWIFI_WARN_PRINT("Invalid wpa number: %d, use default wpa: %d\r\n", args->wpa, chooseWPA);
            /*use default wpa =2*/
            args->wpa = chooseWPA;
        }
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "wpa_key_mgmt=%s\n"
                 "wpa=%d\n",
                 args->auth, args->wpa);

        /* check psk */
        ASSERT(strlen(args->psk), "Invalid psk:%s\r\n", args->psk);
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "wpa_passphrase=%s\n",
                 args->psk);
    }
    else if (strncmp(args->auth, "WPA-EAP", strlen("WPA-EAP")) == 0)
    {
        if (args->wpa > 3)
        {
            LIBWIFI_WARN_PRINT("Invalid wpa number: %d, use default wpa: %d\r\n", args->wpa, chooseWPA);
            /*use default wpa =2*/
            args->wpa = chooseWPA;
        }
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "wpa_key_mgmt=%s\n"
                 "wpa=%d\n",
                 args->auth, args->wpa);
        /* check psk */
        ASSERT(strlen(args->psk), "Invalid psk:%s\r\n", args->psk);
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "wpa_passphrase=%s\n",
                 args->psk);
    }
    else
    {
        // Open System authentication
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "\n\n"); //????
    }

    /* check vendor_elements */
    snprintf(vendorElement, sizeof(vendorElement), "%.2x04%.8x", VENDOR_ELEMENT_ID, args->vendorCode);
    snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "vendor_elements=%s\n", vendorElement);

    /* check hardware mode */
    if ((args->hw_mode != 'a') && (args->hw_mode != 'g'))
    {
        /*use default hw mode = a*/
        LIBWIFI_WARN_PRINT("Invalid hw mode : %c, use default hw mode: %c\r\n", args->hw_mode, chooseHw);
        args->hw_mode = chooseHw;
    }
    if (args->hw_mode == 'a')
    {
        /* check Wi-Fi 5G */
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "hw_mode=%c\n", args->hw_mode);

        /* check channel */
        chooseChannel = args->channel;
        switch (args->channel)
        {
        case 36:
        case 40:
        case 44:
        case 149:
        case 157:
            break;
        default:
            chooseChannel = 36;
            LIBWIFI_WARN_PRINT("Invalid channel number: %d, use default channel: %d\r\n", args->channel, chooseChannel);
            break;
        }
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "channel=%d\n"
                 "ieee80211n=1\n"
                 "ieee80211ac=1\n"
                 "ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]\n"
                 "driver=nl80211\n"
                 "wmm_enabled=1\n"
                 "wpa_pairwise=TKIP CCMP\n"
                 "rsn_pairwise=CCMP\n"
                 "ignore_broadcast_ssid=0\n"
                 "ctrl_interface=/var/run/hostapd\n",
                 chooseChannel);
    }
    else if (args->hw_mode = 'g')
    {
        /* check Wi-Fi 2.4G */
        snprintf(config + strlen(config), (sizeof(config) - strlen(config)), "hw_mode=%c\n", args->hw_mode);
        /* check channel 1~7 */
        chooseChannel = args->channel;
        if (args->channel <= 0 || args->channel >= 8)
        {
            chooseChannel = 1;
            LIBWIFI_WARN_PRINT("Invalid channel number: %d, use default channel: %d\r\n", args->channel, chooseChannel);
        }

        snprintf(config + strlen(config), (sizeof(config) - strlen(config)),
                 "channel=%d\n"
                 "ieee80211n=1\n"
                 "ieee80211ac=0\n"
                 "ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]\n"
                 "driver=nl80211\n"
                 "wme_enabled=1\n"
                 "wpa_pairwise=TKIP CCMP\n"
                 "rsn_pairwise=CCMP\n"
                 "ignore_broadcast_ssid=0\n"
                 "ctrl_interface=/var/run/hostapd\n",
                 chooseChannel);
    }
    else
    {
        ASSERT(NULL, "Hardware Mode not support: %c\r\n", args->hw_mode);
    }

    fprintf(fp, "%s", config);
    fclose(fp);
    fp = NULL;

    /*check IPv4*/
    ASSERT(strlen(args->serverAddress), "Invalid IPv4 server address\r\n");
    /*set IPv4*/
    snprintf(IPv4 + strlen(IPv4), (sizeof(IPv4) - strlen(IPv4)),
             "ifconfig %s %s", args->interface, args->serverAddress);
    LIBWIFI_INFO_PRINT("config ipv4:%s\r\n", IPv4);

    if (args->dhcpEnable == 0) //check  dhcp server status ; 1=enable 0=disalbe
    {
        LIBWIFI_WARN_PRINT("!!! STA setting static IP !!!\r\n");
        system(ifconfig_cmd);
        system(IPv4);
    }
    else
    {
        system(ifconfig_cmd);
        system(IPv4);
        system("udhcpd " UDHCPD_CONFIG_PATH "");
        system("hostapd -B -P " HOSTAPD_PID_PATH " " HOSTAP_CONFIG_PATH " ");
    }

    return 0;
}
int wifiCloseSTA()
{
    char line[LEN] = {0};
    char buffer[LEN] = {0};                             //kill command
    FILE *fp_udhcpc = fopen(UDHCPC_PID_PATH, "r");      //read udhcpc.pid
    FILE *fp_wpa = fopen(WPA_SUPPLICANT_PID_PATH, "r"); //read wpa_supplicant.pid

    if (!fp_udhcpc)
    {
        LIBWIFI_ERROR_PRINT("Cannot open " UDHCPC_PID_PATH "\r\n");
        return -1;
    }
    else
    {
        fgets(line, LEN, fp_udhcpc);
        snprintf(buffer, sizeof(line), "kill %s", line);
        system(buffer); //kill udhcpc
        memset(line, 0, LEN);
        memset(buffer, 0, LEN);
    }
    if (!fp_wpa)
    {
        LIBWIFI_ERROR_PRINT("Cannot open " WPA_SUPPLICANT_PID_PATH "\r\n");
        return -1;
    }
    else
    {
        fgets(line, LEN, fp_wpa);
        snprintf(buffer, sizeof(line), "kill %s", line);
        system(buffer); //kill wpa_supplicant
        memset(line, 0, LEN);
        memset(buffer, 0, LEN);
    }

    fclose(fp_wpa);
    fclose(fp_udhcpc);
    fp_wpa = NULL;
    fp_udhcpc = NULL;

    return 0;
}
int wifiCloseAP()
{
    char line[LEN] = {0};
    char buffer[LEN] = {0};
    FILE *fp_udhcpd = fopen(UDHCPD_PID_PATH, "r");   //read udhcpd.pid
    FILE *fp_hostapd = fopen(HOSTAPD_PID_PATH, "r"); //read hostapd.pid

    if (!fp_hostapd)
    {
        LIBWIFI_ERROR_PRINT("Cannot open " HOSTAPD_PID_PATH "\r\n");
        return -1;
    }
    else
    {
        fgets(line, LEN, fp_hostapd);
        snprintf(buffer, sizeof(line), "kill %s", line);
        system(buffer); //kill hostapd
        memset(line, 0, LEN);
        memset(buffer, 0, LEN);
    }

    if (!fp_udhcpd)
    {
        LIBWIFI_ERROR_PRINT("Cannot open  " UDHCPD_PID_PATH " \r\n");
        return -1;
    }
    else
    {
        fgets(line, LEN, fp_udhcpd);
        snprintf(buffer, sizeof(line), "kill %s", line);
        system(buffer); //kill UDHCPD
        memset(line, 0, LEN);
        memset(buffer, 0, LEN);
    }

    fclose(fp_hostapd);
    fclose(fp_udhcpd);
    fp_hostapd = NULL;
    fp_udhcpd = NULL;

    return 0;
}

int wifiRestartSTA(WifiSTAArgs *args)
{
    wifiCloseSTA();
    return wifiOpenSTA(args);
}
int wifiRestartAP(WifiAPArgs *args)
{
    wifiCloseAP();
    return wifiOpenAP(args);
}
int wifiGetAPList(WifiScanResults *results)
{
    FILE *fp = NULL;
    FILE *fp_bss = NULL;
    char line[256];
    char line_bss[2048];
    char key[1024];
    char value[1024];
    char bss[128] = {0};
    int i = 0;
    int number = 0;
    char *ptr;
    int len;
    char out[512] = {0};
    char vendorCodeBuffer[9] = {0};
    unsigned int elementId;

    ASSERT(results, "Invalid argument\r\n");

    // system("wpa_supplicant -B -Dnl80211 -P /var/run/wpa.pid -iwlan0 -c /mnt/app/etc/wpa_supplicant.conf");
    system("wpa_cli -iwlan0 scan");
    system("wpa_cli -iwlan0 scan_result > " AP_LIST_PATH);

    fp = fopen(AP_LIST_PATH, "r");
    ASSERT(fp, "Cannot open  " AP_LIST_PATH " \r\n");

    fp_bss = fopen("" BSS_LIST_PATH "", "w+r");
    ASSERT(fp_bss, "Cannot open  " BSS_LIST_PATH " \r\n");

    memset(results, 0, sizeof(results));
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        /*count total Wi-Fi AP */
        results->count++;
    }
    fclose(fp);
    fp = NULL;

    LIBWIFI_WARN_PRINT("count:%d\n", results->count -= 1);

    for (i = 0; i < results->count; i++)
    {
        LIBWIFI_DEBUG_PRINT("tag:%d\n", i);
        snprintf(bss, sizeof(bss), "wpa_cli -iwlan0 bss %d >> " BSS_LIST_PATH "", i);
        system(bss);
    }

    while (fgets(line_bss, sizeof(line_bss), fp_bss) != NULL)
    {
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        sscanf(line_bss, "%[^=\n\r]=%[^=\n\r]", key, value);

        if (strncmp(key, "ssid", strlen("ssid")) == 0)
        {
            memset(results->results[number].ssid, 0, sizeof(results->results[number].ssid));
            memcpy(results->results[number].ssid, value, (sizeof(results->results[number].ssid) - 1));
            LIBWIFI_DEBUG_PRINT("AP list number%d SSID =%s\r\n", number, results->results[number].ssid);
            number++;
        }
        if (strncmp(key, "ie", strlen("ie")) == 0)
        {
            ptr = value;
            while (*ptr != 0)
            {
                sscanf(ptr, "%2x", &elementId);
                ptr += 2;
                sscanf(ptr, "%2x", &len);
                ptr += 2;
                len *= 2;
                if (elementId == VENDOR_ELEMENT_ID)
                {
                    memcpy(vendorCodeBuffer, ptr, 8);
                    sscanf(vendorCodeBuffer, "%x", &results->results[number].vendorCode);
                    snprintf(out, sizeof(out), "%.2x04%x", elementId, results->results[number].vendorCode);
                }
                ptr = ptr + len;
            }
        }
        if (strncmp(key, "level", strlen("level")) == 0)
        {
            results->results[number].rssi = 0;
            results->results[number].rssi = atoi(value);
            LIBWIFI_DEBUG_PRINT("AP list number%d level=%d\r\n", number, results->results[number].rssi);
        }
        if (strncmp(key, "freq", strlen("freq")) == 0)
        {
            results->results[number].freq = 0;
            results->results[number].freq = atoi(value);
            LIBWIFI_DEBUG_PRINT("AP list number%d freq=%d\r\n", number, results->results[number].freq);
        }
        if (strncmp(key, "bssid", strlen("bssid")) == 0)
        {
            memset(results->results[number].bssid, 0, sizeof(results->results[number].bssid));
            memcpy(results->results[number].bssid, value, (sizeof(results->results[number].bssid) - 1));
            LIBWIFI_DEBUG_PRINT("AP list number%d Mac Address =%s\r\n", number, results->results[number].bssid);
        }
    }
    fclose(fp_bss);
    fp_bss = NULL;

    // if (results->count > 0)
    // {
    //     wifiPrintAPInformation(wifiGetAPInformation(results, "IoT-lucker"));
    // }
    return 0;
}

WifiScanInfo *wifiGetAPInformation(WifiScanResults *results, const char *ssid)
{
    int i = 0;
    int level_cmp = -1000;
    WifiScanInfo *ret = NULL;

    if (!results)
    {
        LIBWIFI_ERROR_PRINT("Invalid argument\r\n");
    }

    for (i = 0; i < results->count; i++)
    {
        if (strncmp(results->results[i].ssid, ssid, sizeof(results->results[i].ssid)) == 0)
        {
            if (abs(results->results[i].rssi) < abs(level_cmp))
            {
                level_cmp = results->results[i].rssi;
                ret = &results->results[i];
            }
        }
    }

    return ret;
}

int wifiPrintAPInformation(WifiScanInfo *info)
{
    ASSERT(info, "Invalid argument\r\n");

    LIBWIFI_INFO_PRINT("[%s]\n", __FUNCTION__);
    printf("\tssid:%s \n", info->ssid);
    printf("\tMac:%s \n", info->bssid);
    printf("\tlevel:%d \n", info->rssi);
    printf("\tfreq:%d \n", info->freq);
    printf("\tid:%x \n", VENDOR_ELEMENT_ID);
    printf("\tcode:%x \n", info->vendorCode);

    return 0;
}