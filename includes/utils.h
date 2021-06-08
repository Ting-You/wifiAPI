#ifndef _CYP_WIFI_UTILS_H_
#define _CYP_WIFI_UTILS_H_

#include "wifi.h"
/*==================================================================*/
/* Library Functions                                                */
/*==================================================================*/


#define ASSERT(cond, msg, ...)                   \
    if (!(cond))                                 \
    {                                            \
        LIBWIFI_ERROR_PRINT(msg, ##__VA_ARGS__); \
        return -1;                               \
    }

int wifiUtilsGetSTAConfig(WifiSTAArgs *args);
int wifiUtilsSTAInit(WifiSTAArgs *args);
int wifiUtilsGetAPConfig(WifiAPArgs *args);
int wifiUtilsAPInit(WifiAPArgs *args);

#endif // !_CYP_WIFI_UTILS_H_