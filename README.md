android-clm-tracker
===================

Inspired by the performance of native OpenCV code for android, this sample is a straight forward port of OpenCV's face detector sample combined with [Kyle McDonald](http://kylemcdonald.net/)'s maintained C++ FaceTracker code that was originally proposed by [Jason Saragih](http://jsaragih.org/). 

It is available free for non-commercial use, and may be redistributed under these conditions. Please see `license.md` for complete details. For commercial use, [request a quote](http://facetracker.net/quote/).

## Getting it running

These instructions have been tested on Ubuntu 12.04 Linux. I suspect it will work on other architectures. Modify as necessary.

1. Ensure that you have installed what is needed to do OpenCV development on Android. Instructions are [here](http://docs.opencv.org/doc/tutorials/introduction/android_binary_package/android_dev_intro.html). You should only need address the sections under the heading __Manual environment setup for Android development__ and __Native development in C++__.
    - *Note:* I have had success with the Android SDK 2.0 when you are installing the SDK from android-manager.
2. Clone this repository by running `git clone git://github.com/ndepalma/android-clm-tracker.git`. 
3. Import the OpenCV library into your eclipse workspace.
    - You'll find basic instructions [here](http://docs.opencv.org/doc/tutorials/introduction/android_binary_package/O4A_SDK.html).
    - See the section under the head __Import OpenCV library and samples to the Eclipse__
4. Import the project into eclipse by right clicking in the package explorer and selecting Import
    - Accept the default settings and project name.
5. Setup the compilation environment
    - Modify the file `jni/Android.mk`. Change the line `include /home/nd/and_cv/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk` to point to your OpenCV makefile in the Android OpenCV.
    - Modify the file `jni/Application.mk`. Change the line `APP_CPPFLAGS := -frtti -fexceptions -I/home/nd/projects/android-clm-tracker/jni/include/` to point to the absolute path of your android-clm-tracker `jni/include` directory.
    - Right click on the project properties and set up the project to be dependent in the C++ build on the OpenCV Library you imported in Step 3. In the file `project.properties`, it should contain a line that looks something like: `android.library.reference.1=../OpenCV Library - 2.4.9` after this step is completed successfully.
6. Build the `.so` file by changing to the directory of your `android-clm-tracker` and running `ndk-build -B`.
7. Install OpenCV apk to your phone. From [this](http://docs.opencv.org/doc/tutorials/introduction/android_binary_package/O4A_SDK.html) website, find the apk in the OpenCV SDK and install it using: `<Android SDK path>/platform-tools/adb install <OpenCV4Android SDK path>/apk/OpenCV_2.4.9_Manager_2.18_armv7a-neon.apk`
8. Clean and rebuild your eclipse project. There shouldn't be any errors at this point. 
9. Deploy to an attached phone.

---
Errata: 

* Some people have had trouble building the APK. If this is the case, go to your project android build properties and uncheck "Is library". You'll know it because you'll think to yourself "This must refer to OpenCV being a library. Geez, this is a big misunderstanding!"
* If it keeps asking you to install OpenCV Manager and you have - make sure your tablet has the NEON extensions. Find your tablet online and look up the processor. Android apparently lets you install native libraries that aren't for your processor. So make sure that when you install OpenCV and you build your jni (in the android.mk), you are targetting the right processor for your tablet. I have assumed an ARM processor with the NEON (ARMs SIMD Co-processor) extensions.
---
Major credit for this project goes primarily to [Jason Saragih](http://jsaragih.org/) for the original algorithm and code, [Kyle McDonald](http://kylemcdonald.net/) for maintaining the C++ code and mediating the different FaceTracker spin-off projects, and to the [OpenCV](http://opencv.org/) team who originally ported OpenCV to Android. This project is meant to be a proof of concept and, as such, can be extended to your project as needed.

`android-clm-tracker` was forked and modified from the original c++ source at  [Kyle McDonald's FaceTracker github.](https://github.com/kylemcdonald/FaceTracker)
