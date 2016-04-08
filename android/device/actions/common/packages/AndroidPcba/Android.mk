LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
#LOCAL_DEX_PREOPT := false
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-subdir-java-files)\
    src/com/actions/support/ISupportService.aidl

LOCAL_PACKAGE_NAME := PcbaTest
LOCAL_CERTIFICATE := platform
LOCAL_JAVA_LIBRARIES := actions

LOCAL_PROGUARD_FLAG_FILES := proguard.flags

include $(BUILD_PACKAGE)
