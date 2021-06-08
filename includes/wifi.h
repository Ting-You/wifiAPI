#ifndef _CYP_WIFI_CORE_H_
#define _CYP_WIFI_CORE_H_
#include <stdio.h>
#include <stdlib.h>
/********************************************************************/
// Library Constants
/********************************************************************/
/* Define the maximum size of scan results */
#define MAX_WIFI_SCAN_RESULT_SIZE 256
/* Define the maximum size of wifi ssid in scan information */
#define MAX_BSSID_SIZE 64
#define MAX_SSID_SIZE 64
#define MAX_PSK_SIZE 64
#define MAX_VERDOR_SIZE 64
#define MAX_INTERFACE_SIZE 64
#define MAX_AUTH_SIZE 64
#define MAX_IPV4_ADDR_SIZE 64
#define VENDOR_ELEMENT_ID 0xcf
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
/* File Location */
#define HOSTAP_CONFIG_PATH "/mnt/app/etc/hostapd.conf"
#define UDHCPD_CONFIG_PATH "/mnt/app/etc/udhcpd.conf"
#define WPA_CONFIG_PATH "/mnt/app/etc/wpa_supplicant.conf"
#define UDHCPC_PID_PATH "/var/run/udhcpc.pid"
#define UDHCPD_PID_PATH "/var/run/udhcpd.pid"
#define HOSTAPD_PID_PATH "/var/run/hostapd.pid"
#define WPA_SUPPLICANT_PID_PATH "/var/run/wpa.pid"
#define AP_LIST_PATH "/tmp/wifiGetAPList.txt"
#define BSS_LIST_PATH "/tmp/wifibss.txt"
/********************************************************************/
// Library Macros
/********************************************************************/
// #define LIBWIFI_DEBUG_LEVEL                 0       // only shows error message
// #define LIBWIFI_DEBUG_LEVEL                 1       // shows error & warning meesage
#define LIBWIFI_DEBUG_LEVEL 2 // shows error, warning, and debug message
// #define LIBWIFI_DEBUG_LEVEL                 3       // shows error, warning, debug, and trace message
#define LIBWIFI_ERROR_PRINT(fmt, ...) printf(ANSI_COLOR_RED "[wifilib:ERROR:%s:%d] " fmt ANSI_COLOR_RESET, __FILE__, __LINE__, ##__VA_ARGS__)
#define LIBWIFI_INFO_PRINT(fmt, ...) printf(ANSI_COLOR_GREEN "[wifilib:INFO] " fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#if LIBWIFI_DEBUG_LEVEL >= 1
#define LIBWIFI_WARN_PRINT(fmt, ...) printf(ANSI_COLOR_YELLOW "[wifilib:WARN] " fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#else
#define LIBWIFI_WARN_PRINT(fmt, ...)
#endif
#if LIBWIFI_DEBUG_LEVEL >= 2
#define LIBWIFI_DEBUG_PRINT(fmt, ...) printf(ANSI_COLOR_RESET "[wifilib:DEBUG] " fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#else
#define LIBWIFI_DEBUG_PRINT(fmt, ...)
#endif
#if LIBWIFI_DEBUG_LEVEL >= 3
#define LIBWIFI_TRACE_PRINT(fmt, ...) printf(ANSI_COLOR_CYAN "[wifilib:TRACE] " fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#else
#define LIBWIFI_TRACE_PRINT(fmt, ...)
#endif

/*==================================================================*/
// Library Structure
/*==================================================================*/
typedef struct __wifi_scan_info
{
    /*Mac Address*/
    char bssid[MAX_BSSID_SIZE+1];
    /*freq*/
    int freq;
    /* SSID */
    char ssid[MAX_SSID_SIZE + 1];
    /* Signal strength level */
    int rssi;
    /* 4 bytes vendor code in IE (Information Element) field, find by VENDOR_ELEMENT_ID */
    // unsigned int vendorIdentity;
    unsigned int vendorCode;
    /* !!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!! */
    /* If this structure cann't cover all situation, you should add neccesary variables into it */
} WifiScanInfo;

typedef struct __wifi_scan_results
{
    /* scan list */
    struct __wifi_scan_info results[MAX_WIFI_SCAN_RESULT_SIZE];
    /* how many AP scanned */
    int count;
    /* !!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!! */
    /* If this structure cann't cover all situation, you should add neccesary variables into it */
} WifiScanResults;

typedef struct __wifi_sta_arguments
{
    /* !!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!! */
    // FILE *fp;
    char ssid[MAX_SSID_SIZE + 1];
    char psk[MAX_PSK_SIZE];
    /*  */
    char auth[MAX_AUTH_SIZE];

} WifiSTAArgs;

typedef struct __wifi_ap_arguments
{
    /* !!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!! */
    /* wlan0,1,2... */
    char interface[MAX_INTERFACE_SIZE];
    /* WPA-PSk, Empty string */
    char auth[MAX_AUTH_SIZE];
    /* 2,3 */
    int wpa;
    char ssid[MAX_SSID_SIZE + 1];
    char psk[MAX_PSK_SIZE];
    char vendor_elements[MAX_VERDOR_SIZE];
    // unsigned int vendorIdentity;
    unsigned int vendorCode;
    /* b/n/ac,... */
    char hw_mode;
    int channel;
    char serverAddress[MAX_IPV4_ADDR_SIZE];
    /* DHCP */
    int dhcpEnable;
} WifiAPArgs;

/*==================================================================*/
// Library Functions
/*==================================================================*/
/**
 * 
 * \brief Open STA mode connection
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param WifiSTAArgs* Initializing arguments
*/
int wifiOpenSTA(WifiSTAArgs *args);

/**
 * \brief Start AP mode server
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param WifiAPArgs* Initializing arguments
*/
int wifiOpenAP(WifiAPArgs *args);

/**
 * \brief Close STA mode client
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param
*/
int wifiCloseSTA();

/**
 * \brief Close AP mode client
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param
*/
int wifiCloseAP();

/**
 * \brief Restart STA mode client
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param WifiSTAArgs* Initializing arguments
*/
int wifiRestartSTA(WifiSTAArgs *args);

/**
 * \brief Restart AP mode client
 *  
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param WifiAPArgs* Initializing arguments
*/
int wifiRestartAP(WifiAPArgs *args);

/**
 * \brief Get scanned AP list
 *
 * \return int
 *
 * \retval =0 is success, <0 is fail.
 *
 * \param WifiScanResults* Store scanning results
 *
 * \note 
 *      (WifiScanResults *)->count is equal to the number of scanned AP
 *      Scanned informations are stored in (WifiScanResults *)->results[0...count]
*/
int wifiGetAPList(WifiScanResults *results);

/**
 * \brief Get AP information with specific SSID
 *
 * \return WifiScanInfo *
 *
 * \retval 
 *      =NULL if not found.
 *      If an AP with SSID matched, return that (WifiScanInfo *) pointer
 *
 * \param WifiScanResults* Search SSID in the scan results
 * \param char* SSID
 *
*/
WifiScanInfo *wifiGetAPInformation(WifiScanResults *results, const char *ssid);

/**
 * \brief Print beautiful AP broadcast information
 *
 * \return int
 *
 * \retval =0 is success, <0 is not found.
 *
 * \param WifiScanInfo* AP broadcast information
 *
*/
int wifiPrintAPInformation(WifiScanInfo *info);

#endif // !_CYP_WIFI_CORE_H_