# Embedded Linux Wi-Fi Driver 部署

## 目錄

- [開發環境](#devenv)
- [EW-7822ULC Wi-Fi驅動部署](#deploy)
- [啟用 STA 工作模式 (單純連線)](#stamode)
- [啟用 AP 工作模式 (提供連線)](#apmode)
- [Wireless tool的移植](#wirelesstool)

## 開發環境<a href="devenv"></a>

| 項目 | 使用 |
|:-|:-|
|開發版|__Hi3521D v100__|
|原始碼核心|__3.18.y__|
|作業系統|__Ubuntu 14.04 LTS__|
|交叉編譯器|__arm-hisiv600-linux-__|
|Wi-Fi模組|__EW-7822ULC__|
|Wireless tool|__wpa_supplicant-2.6__、__wpa_cli-2.6__、__hostapd-2.8__|

## EW-7822ULC Wi-Fi驅動部署<a href="deploy"></a>

### 取得原始碼

#### Step1:

官方網頁
https://www.edimax.com/edimax/download/download/data/edimax/tw/download/wireless_adapters_ac1200_dual-band/ew-7822ulc

選擇EW-7822ULC Linux Driver並下載![1](C:\Users\tom.jian\Desktop\download.png)

#### Step2:
解壓縮驅動原始碼

```shell
tar zxvf EW-7822ULC_linux_5.8.7.1_kernel_2.6.24-5.3.tar.gz
```

### 將驅動模組加入內核

編輯內核設定

```shell
sudo gedit /home/user/Downloads/hi3531d-toolchain/arm-hisiv600-linux/Hi3531D_SDK_V2.0.3.0/osdrv/opensource/kernel/linux-3.18.y/drivers/net/wireless/Kconfig
#依照自己的原始碼核心路徑
```

加入相關資訊，讓menuconfig找到驅動模組

```shell
#加入
source "/home/user/Desktop/EW-7822ULC_linux_5.8.7.1_kernel_2.6.24-5.3/rtl88x2BU_WiFi_linux_v5.8.7.1_35809.20191129_COEX20191120-7777/Kconfig"
#依照自己的安裝路徑
```

### 修改內核配置

#### Step1: 

切換目錄

```shell
cd /home/user/Downloads/hi3531d-toolchain/arm-hisiv600-linux/Hi3531D_SDK_V2.0.5.0/osdrv/opensource/kernel/linux-3.18.y/
#(依照自己的原始碼核心路徑)
```

#### Step2:

執行

```shell
make ARCH=arm CROSS_COMPILE=arm-hisiv600-linux- menuconfig
```



#### Step3:

修改內核

``` shell
[*] Networking support --->

    <*>   RF switch subsystem support  --->  

	[*]  Wireless --->

		<*>  cfg80211 - wireless configuration API

		[*]  nl80211 testmode command

		[*]  enable powersave by default

		[*]  cfg80211 wireless extensions compatibility *

		<*>  Generic IEEE 802.11 Networking Stack (mac80211)

Device Drivers --->

	[*] Network device support --->

		[*]  Wireless LAN --->

			<M>  IEEE 802.11 for Host AP (Prism2/2.5/3 and WEP/TKIP/CCMP) #關鍵選項

			<M>  Realtek 8822B USB WiFi #內核加入成功會顯示驅動模組名稱
```



### 編譯驅動程式

#### Step1:

修改Makefile建立HISI_3521D平台

```makefile
###################### Platform Related #######################
#加入
CONFIG_PLATFORM_HISI_3521D = y
```

```makefile
ifeq ($(CONFIG_PLATFORM_HISI_3521D), y)

EXTRA_CFLAGS += -DCONFIG_LITTLE_ENDIAN
EXTRA_CFLAGS += -DCONFIG_IOCTL_CFG80211
EXTRA_CFLAGS += -DRTW_USE_CFG80211_STA_EVENT
EXTRA_CFLAGS += -DCONFIG_AP_MODE
EXTRA_CFLAGS += -DRTW_VENDOR_EXT_SUPPORT
ARCH := arm
CROSS_COMPILE ?= arm-hisiv600-linux-
KVER  := 3.18
KSRC := /home/user/Downloads/Hi3521DV100_SDK_V2.0.5.0/osdrv/opensource/kernel/linux-3.18.y/
MODDESTDIR :=/home/user/Downloads/Hi3521DV100_SDK_V2.0.5.0/osdrv/opensource/kernel/linux-3.18.y/drivers/net/wireless/
INSTALL_PREFIX :=
STAGINGMODDIR := /home/user/Downloads/Hi3521DV100_SDK_V2.0.5.0/osdrv/opensource/kernel/linux-3.18.y/drivers/staging/
endif
```

<說明>

- EXTRA_CFLAGS += -DCONFIG_LITTLE_ENDIAN 將內部資料結構定義為LITTLE_ENDIAN
- EXTRA_CFLAGS += -DCONFIG_IOCTL_CFG80211 開啟AP mode必要參數
- EXTRA_CFLAGS += -DRTW_USE_CFG80211_STA_EVENT 開啟AP mode必要參數
- EXTRA_CFLAGS += -DCONFIG_AP_MODE 開啟AP mode功能
- EXTRA_CFLAGS += -DRTW_VENDOR_EXT_SUPPORT  開啟vendor擴展功能
- ARCH 指定運行平台
- CROSS_COMPILE 指定交叉編譯器
- KVER 指定原始碼核心版本
- KSRC 指定原始碼核心路徑
- MODDESTDIR 指定原始碼wireless路徑
- INSTALL_PREFIX 指定安裝路徑
- STAGINGMODDIR 指定staging路徑

#### Step2:

執行

````makefile
make
#產生.ko檔
````

<font color=#FF0000>如產生問題:</font>
```
WARNING: "wireless_send_event" [/home/user/Desktop/EW-7822ULC_linux_5.8.7.1_kernel_2.6.24-5.3/rtl88x2BU_WiFi_linux_v5.8.7.1_35809.20191129_COEX20191120-7777/88x2bu.ko] undefined!
```

解決方法 : 修改內核

```shell
[*] Wireless LAN (IEEE 802.11) ---->

		<M> IEEE 802.11 for Host AP (Prism2/2.5/3 and WEP/TKIP/CCMP)
```



### 重新編譯內核並產生鏡像檔

```shell
make ARCH=arm CROSS_COMPILE=arm-hisiv600-linux- uImage
#uImage路徑:/home/user/Downloads/Hi3521DV100_SDK_V2.0.5.0/osdrv/opensource/kernel/linux-3.18.y/arch/arm/boot
```



### 更新平台內核(以Hi3521D為例)

```shell
version -l
cat /proc/mtd
#檢查燒寫區間
flash_erase /dev/mtd3 0 0
#清除
cat /mnt/nfs/mike/fw_bin/CDPS-P311RN/uImage_hi3521d > /dev/mtdblock3
#寫入
#uImage依照自己的檔案路徑
```

### 掛載並檢查驅動程式

```shell
insmod 88x2bu.ko
###掛載
lsmod
###檢查
##########如果要卸載驅動 rmmod 88x2bu.ko
```

### 檢查並啟用網卡

```shell
ifconfig -a
###顯示wlan0代表掛載成功
ifconfig wlan0 up
###啟動網卡
```

## 啟用 STA 工作模式 (單純連線)<a href="stamode"></a>

#### Step1:

建立wpa_supplicant.conf配置檔並加入參數

```shell
touch /mnt/mtd1/etc/wpa_supplicant.conf
###建立配置檔
vi /mnt/mtd1/etc/wpa_supplicant.conf
###加入以下
ctrl_interface=/var/run/wpa_supplicant
update_config=1
```

#### Step2:

啟動 wpa_supplicant

```shell
wpa_supplicant -B -Dnl80211 -iwlan0 -c /mnt/mtd1/etc/wpa_supplicant.conf
```

#### Step3:

透過wpa_cli設定網路參數

```shell
wpa_cli -i wlan0 add_network
wpa_cli -i wlan0 set_network 0 ssid '"Sony xz3"'
wpa_cli -i wlan0 set_network 0 psk '"0928787207"'
wpa_cli -i wlan0 enable_network 0
```

#### Step4:

設定動態IP

```shell
udhcpc -iwlan0
```

#### Step5:

檢查網卡IP

```shell
ifconfig
```

#### Step6:

檢查連線

```shell
###用手機開瀏覽器打上網卡IP
###板子ping 手機IP位址
ping 192.168.x.x
```

## 啟用 AP 工作模式 (提供連線)<a href="apmode"></a>

### Static IP設定

#### Step1:

建立hostapd使用之配置檔(ap.conf)並加入參數

```shell
touch /mnt/mtd1/etc/ap.conf
###建立配置檔
vi /mnt/mtd1/etc/ap.conf
###加入以下
ssid=IoT-lucker
hw_mode=g
channel=6
interface=wlan0
driver=nl80211
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=lucker123
```

#### Step2:

執行hostapd

```shell
hostapd /mnt/mtd1/etc/ap.conf &
```

#### Step3:

設定網卡靜態IP

```shell
ifconfig wlan0 192.168.3.3
###不要其他網段衝突
```

#### Step4:

檢查連線

```shell
###用手機查看Wi-Fi熱點 "IoT-lucker" 連線後設定靜態IP
###用電腦查看Wi-Fi熱點 "IoT-lucker" 連線後設定靜態IP
ping 192.168.3.x
```

### DHCH Server設定

#### Step1:

建立udhcpd使用之配置檔(udhcpd.conf)並加入參數

````shell
cd /mnt/mtd1/etc
touch  udhcpd.conf
vi udhcpd.conf
###加入以下
start	192.168.3.9	 ###重點修改
end		192.168.3.70 ###重點修改
interface	wlan0	 ###重點修改
opt	dns	8.8.8.8      ###重點修改
option	subnet	255.255.255.0  ###重點修改
opt	router	192.168.3.1   ###重點修改
opt	wins	192.168.10.10
option	dns	129.219.13.81	
option	domain	local
option	lease	864000		# 10 days of seconds
````

#### Step2:

啟動DHCP Server

````shell
udhcpd /mnt/mtd1/etc/udhcpd.conf
````

#### Step3:

執行hostapd

````shell
hostapd /mnt/mtd1/etc/ap.conf &
````

#### Step4:

檢查連線

````shell
###手機或是電腦連上AP，使用DHCP模式，連上後查看是否正確取得IP。
####以上述為例手機取得IP:192.168.3.9
ping 192.168.3.9
````



## Wireless tool的移植<a href="wirelesstool"></a>

下載hostapd、libnl、openssl、wpa_supplicant、wireless tool

| Tool           | Download URL                                                 |
| :------------- | ------------------------------------------------------------ |
| hostapd        | https://w1.fi/releases/                                      |
| libnl          | http://www.linuxfromscratch.org/blfs/view/svn/basicnet/libnl.html |
| openssl        | https://www.openssl.org/source/                              |
| wpa_supplicant | https://w1.fi/releases/                                      |
| wireless tool  | https://hewlettpackard.github.io/wireless-tools/Tools.html   |
| iperf3         | https://iperf.fr/iperf-download.php                          |



### hostapd移植

#### Step1:

如果編譯過wpa_supplicant ，直接跳到Step4

解壓壓縮原始碼

```shell
tar zxvf hostapd-2.8.tar.gz
tar zxvf libnl-3.5.0.tar.gz
tar zxvf openssl-1.1.1d.tar.gz
```

#### Step2:

編譯libnl

```shell
cd /home/user/Desktop/hisi_wifi/libnl-3.5.0
mkdir _intall
./configure CC=arm-hisiv600-linux-gcc --host=arm-hisiv600-linux- --enable-static --enable-shared LIBS=-lpthread --prefix=$(pwd)/_install
sudo apt-get install bison && sudo apt-get install flex
make && make install
```

#### Step3:

編譯openssl

```sh
cd /home/user/Desktop/hisi_wifi/openssl-1.1.1d
./config ARCh=arm no-asm shared --prefix=$(pwd)/_install  CROSS_COMPILE=arm-hisiv600-linux-
sed -i 's/-m64//g' Makefile
make && make install
```

#### Step4:

編譯hostapd

```shell
cd /home/user/Desktop/hisi_wifi/hostapd-2.8/hostapd
cp defconfig .config
vi .config
###修改 #CONFIG_LIBNL32=y ---> CONFIG_LIBNL35=y
###修改 #CONFIG_DRIVER_NL80211=y ---> CONFIG_DRIVER_NL80211=y
vi Makefile
###修改 makefile 指定交叉編譯器 ---> CC=arm-hisiv600-linux-gcc
###加入openssl libnl路徑 
###CFLAGS += -I/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/include
###LIBS+= -L/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/lib
###CFLAGS += -I/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/include
###LIBS += -L/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/lib
make && make install
```

### wpa_supplicant移植

#### Step1:

如果編譯過hostapd，直接跳到Step4

解壓壓縮原始碼

````shell
tar zxvf wpa_supplicant-2.6.tar.gz
tar zxvf libnl-3.5.0.tar.gz
tar zxvf openssl-1.1.1d.tar.gz
````

#### Step2:

編譯libnl

````shell
cd /home/user/Desktop/hisi_wifi/libnl-3.5.0
mkdir _intall
./configure CC=arm-hisiv600-linux-gcc --host=arm-hisiv600-linux- --enable-static --enable-shared LIBS=-lpthread --prefix=$(pwd)/_install
sudo apt-get install bison && sudo apt-get install flex
make && make install
````

#### Step3:

編譯openssl

````shell
cd /home/user/Desktop/hisi_wifi/openssl-1.1.1d
./config ARCh=arm no-asm shared --prefix=$(pwd)/_install  CROSS_COMPILE=arm-hisiv600-linux-
sed -i 's/-m64//g' Makefile
make && make install
````

#### Step4:

編譯wpa_supplicant

````shell
cd /home/user/Desktop/hisi_wifi/wpa_supplicant-2.6/wpa_supplicant
cp defconfig .config
vi .config
###修改 #CONFIG_LIBNL32=y ---> CONFIG_LIBNL35=y
###修改 #CONFIG_DRIVER_NL80211=y ---> CONFIG_DRIVER_NL80211=y
vi Makefile
###修改 makefile 指定交叉編譯器 ---> CC=arm-hisiv600-linux-gcc
###加入openssl libnl路徑 
###CFLAGS += -I/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/include
###LIBS+= -L/home/user/Desktop/hisi_wifi/libnl-3.5.0/_install/lib
###CFLAGS += -I/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/include
###LIBS += -L/home/user/Desktop/hisi_wifi/openssl-1.1.1d/_install/lib
make && make install
````

### iperf3移植

```shell
#!/bin/sh
touch _install

make clean
./configure --host=arm-linux --prefix=$(pwd)/_install  CC=arm-hisiv600-linux-gcc CXX=arm-hisiv600-linux-gcc-g++ ac_cv_func_malloc_0_nonnull=yes CFLAGS=-static CXXFLAGS=-static
make
```







