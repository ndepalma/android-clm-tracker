android-clm-tracker
===================

Proof of concept clm-tracker for Android.

<br/>

This was forked and modified from the original c++ source by kyle mcdonald and algorithm from Jason Saragih. 
Source from [here.](https://github.com/kylemcdonald/FaceTracker)
Requires the NDK.


project.properties: android.library.reference.1=../OpenCV Library - 2.4.9
Android.mk: include /home/nd/and_cv/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk
Application.mk: APP_CPPFLAGS := -frtti -fexceptions -I/home/nd/projects/"OpenCV Sample - face detection"/jni/include/