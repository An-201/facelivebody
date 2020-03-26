LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := seetanet-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../1.seetanet/libs/$(TARGET_ARCH_ABI)/libseetanet.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../1.seetanet/include/
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := openrolezoo-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../0.OpenRoleZoo/obj/local/$(TARGET_ARCH_ABI)/libOpenRoleZoo.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../0.OpenRoleZoo/include/
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := FaceRecognizer514

LOCAL_CFLAGS += -DANDROID_PLATFORM

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/../seeta/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../orz/io/jug/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../orz/utils/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/prebuilt/seetanet/include/

LOCAL_LDFLAGS += -L$(LOCAL_PATH)/lib #-fuse-ld=bfd

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon
endif

LOCAL_LDLIBS += -llog -latomic -lm

LOCAL_SHARED_LIBRARIES += seetanet-prebuilt

LOCAL_STATIC_LIBRARIES += openrolezoo-prebuilt

include $(BUILD_SHARED_LIBRARY)