#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>



using namespace cv;
using namespace std;

Mat imgHSV;
Mat imgThresholded;
Mat imgThresholded_edge;


 int main( int argc, char** argv )
 {





    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  int iLowH = 0;
 int iHighH = 179;

  int iLowS = 0; 
 int iHighS = 255;

  int iLowV = 0;
 int iHighV = 255;

  //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

/*
createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
thresh_callback(0,0);  
*/


    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

    

   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (fill small holes in the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

//what we have now as the thresholded image is very sharp colour detecting image.
  Canny( imgThresholded, imgThresholded_edge, 10 , 30 , 3);

  cvtColor(imgThresholded_edge, imgThresholded_edge, CV_GRAY2BGR);

  imgOriginal+=imgThresholded_edge;
  int count =0 , x_sum=0, y_sum=0 , xavg, yavg;
 
  for( int i=0; i<imgThresholded.rows; i++)
  {
    for(int j=0;j<imgThresholded.cols;j++)
    {
      if(imgThresholded.at<uchar>(i,j) == 255)
      {
        count++;
        x_sum += i;
        y_sum += j;
      }
    }
  }
  if(count == 0)
  {
    count =1000;
  }
  xavg = (x_sum/count);
  yavg = (y_sum/count);

  //cout<< xavg<<" "<<yavg;

  circle( imgOriginal, Point(yavg ,xavg ), 3.0, Scalar( 0, 255, 0 ), -1, 8 );

  imshow("edges image", imgThresholded_edge);
   imshow("Thresholded Image", imgThresholded); //show the thresholded image
  imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

return 0;
 }


