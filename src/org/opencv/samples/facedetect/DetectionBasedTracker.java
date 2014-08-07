package org.opencv.samples.facedetect;

import org.opencv.core.Mat;

public class DetectionBasedTracker
{
    public DetectionBasedTracker(String conName, String triName, String ftName, int minFaceSize) {
        mNativeObj = nativeCreateObject(triName, conName, ftName, minFaceSize);
    }

    public void detect(Mat imageGray, Mat faces) {
        nativeDetect(mNativeObj, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
    }

    public void release() {
        nativeDestroyObject(mNativeObj);
        mNativeObj = 0;
    }

    private long mNativeObj = 0;

    private static native long nativeCreateObject(String triName, String conName, String ftFile, int minFaceSize);
    private static native void nativeDestroyObject(long thiz);
    private static native void nativeDetect(long thiz, long inputImage, long faces);
}
