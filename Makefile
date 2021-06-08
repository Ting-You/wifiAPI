ALLDIRS 		= src test externel_library/wireless-tool/wireless_tools.29/ externel_library/wireless-tool/iw-5.9
DIRS 			= src externel_library/wireless-tool/wireless_tools.29/ externel_library/wireless-tool/iw-5.9
enable_test		:= y


ifeq ($(enable_test),y)
DIRS +=	test
endif

all:
	for i in $(DIRS) ; do make -C $$i $@ || exit $?; done

clean:
	for i in $(ALLDIRS) ; do make -C $$i clean ; done

install:
	for i in $(DIRS) ; do make -C $$i install ; done
