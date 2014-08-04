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
//        DetectionBasedTracker::Parameters DetectorParams;
//        if (faceSize > 0)
//            DetectorParams.minObjectSize = faceSize;
//        result = (jlong)new DetectionBasedTracker(stdFileName, DetectorParams);
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
//            ((DetectionBasedTracker*)thiz)->stop();
//            delete (DetectionBasedTracker*)thiz;
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

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart enter");
    try
    {
      //   ((DetectionBasedTracker*)thiz)->run();
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeStart caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeStart caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStart()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop enter");
    try
    {
      //((DetectionBasedTracker*)thiz)->stop();
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeStop caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeStop caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStop()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize
(JNIEnv * jenv, jclass, jlong thiz, jint faceSize)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize enter");
    try
    {
        if (faceSize > 0)
        {
	  //    DetectionBasedTracker::Parameters DetectorParams =	\
          //  ((DetectionBasedTracker*)thiz)->getParameters();
          //  DetectorParams.minObjectSize = faceSize;
          //  ((DetectionBasedTracker*)thiz)->setParameters(DetectorParams);
        }
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeStop caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeSetFaceSize caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeSetFaceSize()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize exit");
}


JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect
(JNIEnv * jenv, jclass, jlong thiz, jlong imageGray, jlong faces)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect enter");
    try
    {
        //grab image, resize and flip
//        IplImage* I = cvQueryFrame(camera); if(!I)continue; frame = I;
//        if(scale == 1)im = frame;
//        else cv::resize(frame,im,cv::Size(scale*frame.cols,scale*frame.rows));
//        cv::flip(im,im,1); cv::cvtColor(im,gray,CV_BGR2GRAY);

        Mat *gray = (Mat*)imageGray;
	FaceTrackerWrapper *tracker = (FaceTrackerWrapper*)thiz;

        //track this image
        std::vector<int> wSize; if(failed)wSize = wSize2; else wSize = wSize1;
        
	if(tracker->model->Track(*gray,wSize,fpd,nIter,clamp,fTol,fcheck) == 0){
          int idx = tracker->model->_clm.GetViewIdx(); failed = false;
          //Draw(im,model._shape,con,tri,model._clm._visi[idx]);
        }else{
          //if(show){cv::Mat R(im,cvRect(0,0,150,50)); R = cv::Scalar(0,0,255);}
          tracker->model->FrameReset(); failed = true;
        }
        //draw framerate on display image

        //show image and check for user input
	///        imshow("Face Tracker",im);
        //int c = cvWaitKey(10);
        //if(c == 27)break; else if(char(c) == 'd')model.FrameReset();



        //vector<Rect> RectFaces;
        //((DetectionBasedTracker*)thiz)->process(*((Mat*)imageGray));
        //((DetectionBasedTracker*)thiz)->getObjects(RectFaces);
        //vector_Rect_to_Mat(RectFaces, *((Mat*)faces));
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
