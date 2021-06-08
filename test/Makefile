TOP_DIR 		= $(shell /bin/pwd)
INCLUDEDIR 		+= -I$(TOP_DIR)/../includes/
TARGET_PATH		= $(TOP_DIR)/../output
CC              = arm-hisiv600-linux-gcc
LN              = ln
SRCS_AP			= unittestsAP.c
SRCS_STA		= unittestsSTA.c
CFLAGS			+= $(INCLUDEDIR)

OBJS_AP    		= $(SRCS_AP:.c=.o)
OBJS_STA    	= $(SRCS_STA:.c=.o)

LIBS			+= -Wl,--rpath-link $(TARGET_PATH)/lib
LIBS			+= -L$(TARGET_PATH)/lib -lwifi

TARGET_AP			= wifi_unittestsAP
TARGET_STA			= wifi_unittestsSTA
# Compiler flags applied to all files
CFLAGS += -g2 -Os -Wall -D_POSIX -D_DEBUG -D_VERBOSE -D_MT_CONNECTION_MANAGER -D_MT_RENDERINGCONTROL -D_MT_AVTRANSPORT -D_GNU_SOURCE

all : wifi_unittestsAP wifi_unittestsSTA

wifi_unittestsAP : $(OBJS_AP)
	$(CC) $(CFLAGS) -o ${TARGET_AP} $(OBJS_AP) $(LIBS) $(LDLIBS)

wifi_unittestsSTA : $(OBJS_STA)
	$(CC) $(CFLAGS) -o ${TARGET_STA} $(OBJS_STA) $(LIBS) $(LDLIBS)

clean :
	sudo rm -f $(OBJS_STA) ${TARGET_STA} $(OBJS_AP) ${TARGET_AP}
 	# sudo rm -f $(TARGET_PATH)/bin/*

install:
	# $(STRIP) $(TARGET)
	sudo install -m 755 $(TARGET_AP) $(TARGET_STA) $(TARGET_PATH)/bin
