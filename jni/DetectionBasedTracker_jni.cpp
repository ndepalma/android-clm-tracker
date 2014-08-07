#include <DetectionBasedTracker_jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
#include <FaceTracker/Tracker.h>

#include <string>
#include <vector>
#include <iostream>

#include <android/log.h>

#define LOG_TAG "FaceDetection/DetectionBasedTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

struct FaceTrackerWrapper_struct {
  FACETRACKER::Tracker *model;
  cv::Mat *tri;
  cv::Mat *con;
};

typedef FaceTrackerWrapper_struct FaceTrackerWrapper;

std::vector<int> wSize1(1); 
std::vector<int> wSize2(3);
int nIter = 5; double clamp=3,fTol=0.01;
bool fcheck = false; double scale = 1; int fpd = -1; bool show = true;
bool failed = true;

  
JNIEXPORT jlong JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject
(JNIEnv * jenv, jclass, jstring jTriFileName, jstring jConFile, jstring jFtFile, jint faceSize)
{
    wSize1[0] = 7;
    wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject enter");
    const char* jtrifilestr = jenv->GetStringUTFChars(jTriFileName, NULL);
    const char* jconfilestr = jenv->GetStringUTFChars(jConFile, NULL);
    const char* jftfilestr = jenv->GetStringUTFChars(jFtFile, NULL);
    string stdFtFileName(jftfilestr);
    string stdTriFileName(jtrifilestr);
    string stdConFileName(jconfilestr);
    jlong result = 0;
    FaceTrackerWrapper *wrapper = new FaceTrackerWrapper;
    try
    {
      //set other tracking parameters
      LOGD("Loading file... %s\n", stdFtFileName.c_str());
      wrapper->model = new FACETRACKER::Tracker(stdFtFileName.c_str());
      LOGD("Loading file... %s\n", stdTriFileName.c_str());
      wrapper->tri = new Mat(FACETRACKER::IO::LoadTri(stdTriFileName.c_str()));
      LOGD("Loading file... %s\n", stdConFileName.c_str());
      wrapper->con = new Mat(FACETRACKER::IO::LoadCon(stdConFileName.c_str()));

      result = (jlong)wrapper;
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeCreateObject caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeCreateObject()");
        return 0;
    }

    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject exit");
    return result;
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject enter");
    try
    {
        if(thiz != 0)
        {
	        FaceTrackerWrapper *wrapper = (FaceTrackerWrapper*)thiz;
	        delete wrapper->con;
	        delete wrapper->tri;
	        delete wrapper->model;
	        delete wrapper;
        }
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeestroyObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeDestroyObject caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeDestroyObject()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect
(JNIEnv * jenv, jclass, jlong thiz, jlong imageGray, jlong faces)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect enter");
    try
    {
        //grab image
        Mat *gray = (Mat*)imageGray;
    	FaceTrackerWrapper *tracker = (FaceTrackerWrapper*)thiz;

        std::vector<int> wSize; if(failed)wSize = wSize2; else wSize = wSize1;
        
	    if(tracker->model->Track(*gray,wSize,fpd,nIter,clamp,fTol,fcheck) == 0){
            int idx = tracker->model->_clm.GetViewIdx(); failed = false;
        }else{
            tracker->model->FrameReset(); failed = true;
        }
        
	    *((Mat*)faces) = tracker->model->_shape;
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeDetect caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code DetectionBasedTracker.nativeDetect()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect exit");
}
