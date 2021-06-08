# User Guide of libwifi

## ● Outline

- Prerequisite
- Library

## ● Prerequisite

- Wi-Fi driver porting
For details, please refer to another document.


- Tools Download URL

| Tool           | Download URL                                                 |
| :------------- | ------------------------------------------------------------ |
| hostapd        | https://w1.fi/releases/                                      |
| libnl          | http://www.linuxfromscratch.org/blfs/view/svn/basicnet/libnl.html |
| openssl        | https://www.openssl.org/source/                              |
| wpa_supplicant | https://w1.fi/releases/                                      |
| wireless tools | https://hewlettpackard.github.io/wireless-tools/Tools.htmliw |
| iw             | https://www.linuxfromscratch.org/blfs/view/svn/basicnet/iw.html |


- hostapd tool

````shell
cd /home/user/Desktop/hisi_wifi/hostapd-2.8/hostapd
cp defconfig .config

vi .config
#-----------------------------Modify config-----------------------------------#
#CONFIG_LIBNL32=y ---> CONFIG_LIBNL35=y
#CONFIG_DRIVER_NL80211=y ---> CONFIG_DRIVER_NL80211=y 
#CONFIG_IEEE80211AC=y ---> CONFIG_IEEE80211AC=y
#CONFIG_IEEE80211AX=y ---> CONFIG_IEEE80211AX=y
#-------------------------------------END-------------------------------------#

vi Makefile
#-----------------------------Modify Makefile---------------------------------#
###Setting cross compiler
CC=arm-hisiv600-linux-gcc

###Add openssl and libnl paths
CFLAGS += -I/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/include
LIBS += -L/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/lib
CFLAGS += -I/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/include
LIBS += -L/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/lib
#-------------------------------------END-------------------------------------#

make clean && make && make install
````

- wpa_supplicant tool

````shell
cd /home/user/Desktop/hisi_wifi/wpa_supplicant-2.6/wpa_supplicant
cp defconfig .config
vi .config
#-----------------------------Modify config-----------------------------------#
#CONFIG_LIBNL32=y ---> CONFIG_LIBNL35=y
#CONFIG_DRIVER_NL80211=y ---> CONFIG_DRIVER_NL80211=y
#-------------------------------------END-------------------------------------#

vi Makefile
#-----------------------------Modify Makefile---------------------------------#
###Setting cross compiler
CC=arm-hisiv600-linux-gcc

###Add openssl and libnl paths
CFLAGS += -I/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/include
LIBS+= -L/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/lib
CFLAGS += -I/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/include
LIBS += -L/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/lib
#-------------------------------------END-------------------------------------#

make clean && make && make install
````

- wireless tool

````shell
cd /home/user/Desktop/hisi_wifi/wireless_tools.29/_install
mkdir _install

vi Makefile
#-----------------------------Modify Makefile---------------------------------#
###install path
PREFIX = /home/user/Desktop/hisi_wifi/wireless_tools.29/_install

###Setting cross compiler
CC=arm-hisiv600-linux-gcc
AR=arm-hisiv600-linux-ar
RANLIB=arm-hisiv600-linux-ranlib
#-------------------------------------END-------------------------------------#

make clean && make && make install 
````

- openssl

````shell
cd /home/user/Desktop/hisi_wifi/openssl-1.1.1d
mkdir _install
./config ARCH=arm no-asm shared --prefix=$(pwd)/_install  CROSS_COMPILE=arm-hisiv600-linux-
sed -i 's/-m64//g' Makefile
make clean && make && make install
````

- libnl

````shell
cd /home/user/Desktop/hisi_wifi/libnl-3.5.0
mkdir _intall
./configure CC=arm-hisiv600-linux-gcc --host=arm-hisiv600-linux- --enable-static --enable-shared LIBS=-lpthread --prefix=$(pwd)/_install
sudo apt-get install bison && sudo apt-get install flex
make clean && make && make install
````

- iw

```shell
cd /home/user/Desktop/hisi_wifi/iw-5.9
mkdir _install

vi Makefile
#-----------------------------Modify Makefile---------------------------------#
PREFIX ?= $(PWD)/_install
CFLAGS += -I/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/include
LDFLAGS += -L/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/lib
#-------------------------------------END-------------------------------------#

make clean && make CC=arm-hisiv600-linux-gcc && make install 
```

  

## ● Library



<h1>Compile</h1>

Please note the following before compiling.

- Need to change your cross compiler. (CC = your cross compiler)
- ALLDIR and DIR can add libraries to be compiled. (if necessary)
- The compiled files are in the output directory.

Example: make clean && make && make install



<h1>Structure</h1>



- <h2>WifiSTAArgs</h2>



<h2>Description:</h2>

Setting wpa_supplicant.conf parameters.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

#define MAX_SSID_SIZE 64
#define MAX_PSK_SIZE 64
#define MAX_AUTH_SIZE 64

typedef struct __wifi_sta_arguments
{
    char ssid[MAX_SSID_SIZE + 1];
    char psk[MAX_PSK_SIZE];
    char auth[MAX_AUTH_SIZE];
} WifiSTAArgs;

```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

| **name** | **Introduction**                         |
| -------- | ---------------------------------------- |
| ssid     | Set the SSID of wpa_supplicant.conf.     |
| psk      | Set the PSK of wpa_supplicant.conf.      |
| auth     | Set the Key_mgmt of wpa_supplicant.conf. |




- <h1>WifiAPArgs</h1>


<h2>Description:</h2>

Setting hostapd.conf parameters.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

#define MAX_INTERFACE_SIZE 64
#define MAX_AUTH_SIZE 64
#define MAX_SSID_SIZE 64
#define MAX_PSK_SIZE 64
#define MAX_VERDOR_SIZE 64
#define MAX_IPV4_ADDR_SIZE 64

typedef struct __wifi_ap_arguments
{
    char interface[MAX_INTERFACE_SIZE];
    char auth[MAX_AUTH_SIZE];
    int wpa;
    char ssid[MAX_SSID_SIZE + 1];
    char psk[MAX_PSK_SIZE];
    char vendor_elements[MAX_VERDOR_SIZE];
    unsigned int vendorCode;
    char hw_mode;
    int channel;
    char serverAddress[MAX_IPV4_ADDR_SIZE];
    int dhcpEnable;
} WifiAPArgs;

```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

| name            | Introduction                        |
| --------------- | ----------------------------------- |
| interface       | Set the interface of hostapd.       |
| auth            | Set the wpa_key_mgmt of hostapd.    |
| wpa             | Set the wpa of hostapd.             |
| ssid            | Set the ssid of hostapd.            |
| psk             | Set the wpa_passphrase of hostapd.  |
| vendor_elements | Set the vendor_elements of hostapd. |
| vendorCode      | Set the vendorCode.                 |
| hw_mode         | Set the hw_mode of hostapd.         |
| channel         | Set the channel of hostapd.         |
| serverAddress   | Set IPv4 address.                   |
| dhcpEnable      | Set whether to enable dhcp          |






- <h1>WifiScanResults</h1>



<h2>Description:</h2>

- Record the total number of scanned AP

- Save all AP information.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

#define MAX_WIFI_SCAN_RESULT_SIZE 256

typedef struct __wifi_scan_results
{
    struct __wifi_scan_info results[MAX_WIFI_SCAN_RESULT_SIZE];
    int count;
} WifiScanResults;

```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

| name    | Introduction                           |
| ------- | -------------------------------------- |
| results | Save all AP information.               |
| count   | Record the total number of scanned AP. |





- <h1>WifiScanInfo</h1>



<h2>Description:</h2>

Save AP information.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

#define MAX_BSSID_SIZE 64
#define MAX_SSID_SIZE 64

typedef struct __wifi_scan_info
{
    char bssid[MAX_BSSID_SIZE+1];
    int freq;
    char ssid[MAX_SSID_SIZE + 1];
    int rssi;
    unsigned int vendorCode;
} WifiScanInfo;

```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

| name           | Introduction                |
| -------------- | --------------------------- |
| **bssid**      | **AP Mac Address**          |
| **freq**       | **AP Frequency**            |
| **ssid**       | **AP SSID**                 |
| **rssi**       | **AP Single level**         |
| **vendorCode** | **AP Vendor elements code** |





<h1>Function</h1>



- <h2>wifiOpenSTA( )</h2>



<h2>Description:</h2>

Create new wpa_supplicant.conf, wpa_supplicant.pid and connect to Wi-Fi AP.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

int wifiOpenSTA(WifiSTAArgs *args);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

**WifiSTAArgs* Initializing arguments.**



<h2>Example(Open System Authentication):</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    WifiSTAArgs staargs;
    /* init */
    memset(&staargs, 0, sizeof(WifiSTAArgs));
    memcpy(staargs.ssid, "IoT-lucker", strlen("IoT-lucker") + 1);

    wifiOpenSTA(&input);

    return 0;
}

// output: wpa_supplicant.conf
// ctrl_interface=/var/run/wpa_supplicant
// update_config=1
// network={
//     ssid="IoT-lucker"
//     key_mgmt=NONE
//    
// }
```



<h2>Example(Shared Key Authentication):</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    WifiSTAArgs staargs;
    /* init */
    memset(&staargs, 0, sizeof(WifiSTAArgs));
    memcpy(staargs.ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(staargs.psk, "lucker123", strlen("lucker123") + 1);
    memcpy(staargs.auth, "WPA-PSK", strlen("WPA-PSK") + 1);

    wifiOpenSTA(&input);

    return 0;
}

// output: wpa_supplicant.conf
// ctrl_interface=/var/run/wpa_supplicant
// update_config=1
// network={
//     ssid="IoT-lucker"
//     psk="lucker123"
//     key_mgmt=WPA-PSK
//     pairwise=CCMP
// }
```



<h2>Return Value:</h2>

- On success, wifiOpenSTA( ) return 0.

- On error, wifiOpenSTA( )  return -1.





- <h1>wifiOpenAP( )</h1>



<h2>Description:</h2>

Create new hostapd.conf, hostapd.pid and Wi-Fi AP.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

int wifiOpenAP(WifiAPArgs *args);
```

Compile and link with -lwifi.



<h2>Parameter Description</h2>

**WifiAPArgs* Initializing arguments.**

| Name          | Description                                                  | Note                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| SSID          | You can set SSID.                                            | Can not be empty.                                            |
| PSK           | You can set PSK.                                             |                                                              |
| Interface     | You can use the ifconfig command to confirm the interface.   |                                                              |
| WPA           | Enable WPA, setting this variable configures the AP to require WPA, If the wrong parameter is set, the default parameter will be enabled. | The default value is 2                                       |
| hw_mode       | a=5G mode, g=2.4G mode. If you set the wrong mode, the default mode will be used. | The default mode is 5G mode.                                 |
| channel       | You can use iwlist to view the available channels. If you set the wrong channel, the default channel will be used. | 5G default channel is 36.                                2.4G default channel is 1. |
| vendorCode    | You can customize 8 bits vendorcode.                         |                                                              |
| auth          | You can set the  key management algorithms.                  |                                                              |
| serverAddress | Set a set of static IP. If there are other network interfaces on the board, please make sure that there is no IP conflict. |                                                              |
| dhcpEnable    | 1=enable, 0=disable. If the number you set is not 0, it will be enabled by default. | If set to 0, the STA must set a static IP.                   |





<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    WifiAPArgs apargs;

    /* init */
    memset(&apargs, 0, sizeof(apargs));
    memcpy(apargs.ssid, "IoT-lucker", strlen("IoT-lucker") + 1);
    memcpy(apargs.psk, "lucker123", strlen("lucker123") + 1);
    memcpy(apargs.interface, "wlan0", strlen("wlan0") + 1);
    apargs.wpa = 2;
    apargs.hw_mode = 'g';
    apargs.channel = 1;
    apargs.vendorCode = 0x11223301;
    memcpy(apargs.auth, "WPA-PSK", strlen("WPA-PSK") + 1);
    memcpy(apargs.serverAddress, "192.168.3.1", strlen("192.168.3.1") + 1);
    apargs.dhcpEnable = 1;

    wifiOpenAP(&input);

    return 0;
}

//output: hostapd.conf
//ssid=IoT-lucker
//interface=wlan0
//wpa_key_mgmt=WPA-PSK
//wpa=2
//wpa_passphrase=lucker123
//vendor_elements=cf0411223301
//hw_mode=g
//channel=1
//ieee80211n=1
//ieee80211ac=0
//ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]
//driver=nl80211
//vwme_enabled=1
//wpa_pairwise=CCMP
//rsn_pairwise=CCMP
//ignore_broadcast_ssid=0
//ctrl_interface=/var/run/hostapd
```






<h2>Return value:</h2>

- On success, wifiOpenAP( ) return 0.

- On error, wifiOpenAP( )  return -1.





- <h1>wifiCloseSTA( )</h1>



<h2>Description:</h2>

- Read udhcpc.pid and wpa_supplicant.pid files.

- Kill process by files content.



<h2>Synopsis:</h2>

```C 
#include<wifi.h>

int wifiCloseSTA();
```

Compile and link with -lwifi.



<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    wifiCloseSTA();

    return 0;
}
```



<h2>Return Value:</h2>

- On success, wifiCloseSTA( ) return 0.

- On error, wifiCloseSTA( )  return -1.





- <h1>wifiCloseAP( )</h1>



<h2>Description:</h2>

- Read udhcpd.pid and hostapd.pid files.

- Kill process by files content.



<h2>Synopsis:</h2>

```C 
#include<wifi.h>

int wifiCloseAP();
```

Compile and link with -lwifi.



<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    wifiCloseAP();

    return 0;
}
```



<h2>Return value:</h2>

- On success, wifiCloseAP( ) return 0.

- On error, wifiCloseAP( )  return -1.





- <h1>wifiRestartSTA( )</h1>



<h2>Description:</h2>

Restart STA Mode.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

int wifiRestartSTA(WifiSTAArgs *args);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

**WifiSTAArgs* Initializing arguments.**



<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    wifiRestartSTA();

    return 0;
}
```



<h2>Return Value:</h2>	

- On success, wifiRestartSTA( ) return 0.

- On error, wifiRestartSTA( )  return -1.





- <h1>wifiRestartAP( )</h1>



<h2>Description:</h2>

Restart AP Mode.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

int wifiRestartAP(WifiSTAArgs *args);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

**WifiAPArgs* Initializing arguments.**



<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    wifiRestartAP();

    return 0;
}
```



<h2>Return value:</h2>

- On success, wifiRestartAP( ) return 0.

- On error, wifiRestartAP( )  return -1.





- <h1>wifiGetAPList( )</h1>



<h2>Description:</h2>

- Get scanned AP list.

- Save all AP information.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

  int wifiGetAPList(WifiScanResults *results);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

**WifiScanResults* save all AP information.**



<h2>Example:</h2>

Please refer to wifiPrintAPInformation( ) function.



<h2>Return Value:</h2>

- On success, wifiGetAPList( ) return 0.

- On error, wifiGetAPList( )  return -1.




- <h1>wifiGetAPInformation( )</h1>



<h2>Description:</h2>

Get AP information with specific SSID.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

WifiScanInfo *wifiGetAPInformation(WifiScanResults *results, const char *ssid);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

| name                     | Introduction                     |
| ------------------------ | -------------------------------- |
| WifiScanResults* results | Search SSID in the scan results. |
| const char* ssid         | Can not be empty.                |



<h2>Example:</h2>

Please refer to wifiPrintAPInformation( ) function.



<h2>Return Value:</h2>

- If an AP with SSID matched, wifiGetAPInformation( ) return that (WifiScanInfo *) pointer.

- If not found, wifiGetAPInformation( )  return NULL.





- <h1>wifiPrintAPInformation( )</h1>



<h2>Description:</h2>

Print AP information.



<h2>Synopsis:</h2>

```c
#include<wifi.h>

int wifiPrintAPInformation(WifiScanInfo *info);
```

Compile and link with -lwifi.



<h2>Parameter Description:</h2>

**WifiScanInfo* AP  information.** 



<h2>Example:</h2>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"

int main()
{
    WifiScanResults args;

    wifiGetAPList(&args);

    wifiPrintAPInformation(wifiGetAPInformation(&args,"IoT-lucker"));    

    return 0;
}
```



<h2>Return Value:</h2>

- On success, wifiPrintAPInformation( ) return 0.

- On error, wifiPrintAPInformation( )  return -1.



<h2>Note:</h2>

This function must be used with wifiGetAPList( ) function and wifiGetAPInformation( ) function.











