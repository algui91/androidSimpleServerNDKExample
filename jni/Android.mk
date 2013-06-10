LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sServer
LOCAL_SRC_FILES := sServer.c
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)