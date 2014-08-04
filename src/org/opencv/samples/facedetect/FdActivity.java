package org.opencv.samples.facedetect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;

public class FdActivity extends Activity implements CvCameraViewListener2 {

    private static final String    TAG                 = "OCVSample::Activity";
    private static final Scalar    FACE_RECT_COLOR     = new Scalar(0, 255, 0, 255);
    public static final int        JAVA_DETECTOR       = 0;
    public static final int        NATIVE_DETECTOR     = 1;

    private MenuItem               mItemFace50;
    private MenuItem               mItemFace40;
    private MenuItem               mItemFace30;
    private MenuItem               mItemFace20;
    private MenuItem               mItemType;

    private Mat                    mRgba;
    private Mat                    mGray;
//    private File                   mCascadeFile;
    private CascadeClassifier      mJavaDetector;
    private DetectionBasedTracker  mNativeDetector;

    private int                    mDetectorType       = NATIVE_DETECTOR;
    private String[]               mDetectorName;

    private float                  mRelativeFaceSize   = 0.2f;
    private int                    mAbsoluteFaceSize   = 0;

    private CameraBridgeViewBase   mOpenCvCameraView;

    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
    	private File resetResource(int id, String filename) throws IOException {
    		// load cascade file from application resources
            InputStream is = getResources().openRawResource(id);
            File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
            File cascadeFile = new File(cascadeDir, filename);
            FileOutputStream os = new FileOutputStream(cascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
            return cascadeFile;
    	}
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");

                    // Load native library after(!) OpenCV initialization
                    System.loadLibrary("detection_based_tracker");

                    try {
                        
                        File mCascadeFile = resetResource(R.raw.lbpcascade_frontalface, "lbpcascade_frontalface.xml");

                        mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
                        if (mJavaDetector.empty()) {
                            Log.e(TAG, "Failed to load cascade classifier");
                            mJavaDetector = null;
                        } else
                            Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

                        File triFile, conFile, ftFile;
                        
                        triFile = resetResource(R.raw.face_tri, "face_tri.tri");
                        conFile = resetResource(R.raw.face_con, "face_con.con");
                        ftFile = resetResource(R.raw.face_tr2, "face_tr2.tracker");
                        
                        mNativeDetector = new DetectionBasedTracker(conFile.getAbsolutePath(), triFile.getAbsolutePath(), ftFile.getAbsolutePath(), 0);

//                        cascadeDir.delete();

                    } catch (IOException e) {
                        e.printStackTrace();
                        Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
                    }

                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public FdActivity() {
        mDetectorName = new String[2];
        mDetectorName[JAVA_DETECTOR] = "Java";
        mDetectorName[NATIVE_DETECTOR] = "Native (tracking)";

        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.face_detect_surface_view);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.fd_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        mGray = new Mat();
        mRgba = new Mat();
    }

    public void onCameraViewStopped() {
        mGray.release();
        mRgba.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {

        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();

        if (mAbsoluteFaceSize == 0) {
            int height = mGray.rows();
            if (Math.round(height * mRelativeFaceSize) > 0) {
                mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
            }
            mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);
        }

        MatOfRect faces = new MatOfRect();
        Mat faces2 = new Mat();

        if (mDetectorType == JAVA_DETECTOR) {
            if (mJavaDetector != null)
                mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
                        new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
        }
        
        else if (mDetectorType == NATIVE_DETECTOR) {
            if (mNativeDetector != null)
                mNativeDetector.detect(mGray, faces2);
//            String out = faces2.dump();
            
//            Log.i(TAG, "MAT: " + out);
        }
        else {
            Log.e(TAG, "Detection method is not selected!");
        }
        Draw(mRgba,faces2);
//        Draw();
//        Rect[] facesArray = faces.toArray();
//        for (int i = 0; i < facesArray.length; i++)
//            Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);

        return mRgba;
    }
    
//    private void Draw(Mat image,Mat shape,Mat con,Mat tri,Mat visi)
    private void Draw(Mat image,Mat shape)
    	{
    	  int i,n = shape.rows()/2; Point p1,p2; Scalar c;

    	  //draw triangulation
//    	  c = CV_RGB(0,0,0);
//    	  for(i = 0; i < tri.rows(); i++){
//    	    if(visi.at<int>(tri.at<int>(i,0),0) == 0 ||
//    	       visi.at<int>(tri.at<int>(i,1),0) == 0 ||
//    	       visi.at<int>(tri.at<int>(i,2),0) == 0)continue;
    		  
//    	    p1 = Core.point(shape.at<double>(tri.at<int>(i,0),0),
//    			   shape.at<double>(tri.at<int>(i,0)+n,0));
//    	    p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
//    			   shape.at<double>(tri.at<int>(i,1)+n,0));
//    	    cv::line(image,p1,p2,c);
//    	    p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
//    			   shape.at<double>(tri.at<int>(i,0)+n,0));
//    	    p2 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
//    			   shape.at<double>(tri.at<int>(i,2)+n,0));
//    	    cv::line(image,p1,p2,c);
//    	    p1 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
//    			   shape.at<double>(tri.at<int>(i,2)+n,0));
//    	    p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
//    			   shape.at<double>(tri.at<int>(i,1)+n,0));
//    	    cv::line(image,p1,p2,c);

    		  //    	  }
    	  //draw connections
    	  
//    		  c = CV_RGB(0,0,255);
//    	  for(i = 0; i < con.cols; i++){
//    	    if(visi.at<int>(con.at<int>(0,i),0) == 0 ||
//    	       visi.at<int>(con.at<int>(1,i),0) == 0)continue;
//    	    p1 = cv::Point(shape.at<double>(con.at<int>(0,i),0),
//    			   shape.at<double>(con.at<int>(0,i)+n,0));
//    	    p2 = cv::Point(shape.at<double>(con.at<int>(1,i),0),
//    			   shape.at<double>(con.at<int>(1,i)+n,0));
//    	    cv::line(image,p1,p2,c,1);
//    	  }
    		  
    		
    		  c = new Scalar(255, 0, 0);
    	  //draw points
    	  for(i = 0; i < n; i++){
//    	    if(visi.at<int>(i,0) == 0)continue;
    		  	
    	    p1 = new Point((double)shape.get(i,0)[0],(double)shape.get(i+n,0)[0]);
    	    
    	    Core.circle(image, p1, 2, c);
//    	  }
    	}
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");
        mItemFace50 = menu.add("Face size 50%");
        mItemFace40 = menu.add("Face size 40%");
        mItemFace30 = menu.add("Face size 30%");
        mItemFace20 = menu.add("Face size 20%");
        mItemType   = menu.add(mDetectorName[mDetectorType]);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
        if (item == mItemFace50)
            setMinFaceSize(0.5f);
        else if (item == mItemFace40)
            setMinFaceSize(0.4f);
        else if (item == mItemFace30)
            setMinFaceSize(0.3f);
        else if (item == mItemFace20)
            setMinFaceSize(0.2f);
        else if (item == mItemType) {
            int tmpDetectorType = (mDetectorType + 1) % mDetectorName.length;
            item.setTitle(mDetectorName[tmpDetectorType]);
            setDetectorType(tmpDetectorType);
        }
        return true;
    }

    private void setMinFaceSize(float faceSize) {
        mRelativeFaceSize = faceSize;
        mAbsoluteFaceSize = 0;
    }

    private void setDetectorType(int type) {
        if (mDetectorType != type) {
            mDetectorType = type;

            if (type == NATIVE_DETECTOR) {
                Log.i(TAG, "Detection Based Tracker enabled");
                mNativeDetector.start();
            } else {
                Log.i(TAG, "Cascade detector enabled");
                mNativeDetector.stop();
            }
        }
    }
}
