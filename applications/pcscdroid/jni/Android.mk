LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS     := -llog -L$(SYSROOT)/usr/lib 

include $(CLEAR_VARS)

include $(LOCAL_PATH)/pcsc-lite-1.8.6/Android.mk \
	$(LOCAL_PATH)/libusb-1.0.3/Android.mk \
	$(LOCAL_PATH)/ccid-1.4.7/Android.mk