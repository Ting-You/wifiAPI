TOP_DIR			= $(shell /bin/pwd)
INCLUDEDIR		= -I$(TOP_DIR)/../includes
TARGET_PATH		= $(TOP_DIR)/../output
CC              = arm-hisiv600-linux-gcc
LN              = ln
SRCS			= wifi.c utils.c

CFLAGS			+= $(INCLUDEDIR)

OBJS    		= $(SRCS:.c=.o)

LIBS			+= -Wl,--rpath-link $(TARGET_PATH)/lib
LIBS			+= -L$(TARGET_PATH)/lib

TARGET			= libwifi.so

# Compiler flags applied to all files
CFLAGS			+= -shared -fPIC -nostartfiles
CFLAGS 			+= -D_GNU_SOURCE # fix implicit declaration of function

all : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS) $(LDLIBS)

# Macro rule for all object files.

clean :
	sudo rm -f $(OBJS) $(TARGET)
	# sudo rm -f $(TARGET_PATH)/lib/*

install:
	# $(STRIP) $(TARGET)
	sudo install -m 755 $(TARGET) $(TARGET_PATH)/lib
