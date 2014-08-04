LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include /home/nd/and_cv/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := DetectionBasedTracker_jni.cpp \
	src/lib/IO.cc\
	src/lib/PDM.cc\
	src/lib/Patch.cc\
	src/lib/CLM.cc\
	src/lib/FDet.cc\
	src/lib/PAW.cc\
	src/lib/FCheck.cc\
	src/lib/Tracker.cc
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := detection_based_tracker

include $(BUILD_SHARED_LIBRARY)
