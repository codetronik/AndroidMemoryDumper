LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := dump
LOCAL_SRC_FILES := main.cpp

include $(BUILD_EXECUTABLE)

all: post-build

post-build:
	adb push ../libs/$(TARGET_ARCH_ABI)/$(LOCAL_MODULE) /data/local/tmp
	adb shell "chmod 777 /data/local/tmp/$(LOCAL_MODULE)"