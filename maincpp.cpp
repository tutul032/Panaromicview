#include "opencv2/opencv.hpp"
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace cv;

int main(int, char**)
{
	VideoCapture cap("rtsp://root:root@10.10.2.2/live.sdp"); // open the default camera
	//VideoCapture cap("MyVideo.avi"); // open the default camera

	if (!cap.isOpened())  // check if we succeeded
		return -1;
	int frames_per_second = 15; 
	VideoWriter oVideoWriter("D:/MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),
		frames_per_second, Size(1900, 500), true); // saved video format and size in D drive location
	
	for (;;)
	{
		Mat frame, dst, src;
		
		 // get a new frame from camera
		cap >> frame;
		resize(frame, src, Size(1900, 1900)); // resized original video  

		logPolar(frame, dst, Point(src.cols / 2, src.rows / 2), 280, INTER_CUBIC); // conversion from cartesian to polar coordinate 

		cv::Point2f pc(dst.cols / 2., dst.rows / 2.);
		cv::Mat r = cv::getRotationMatrix2D(pc, 90, 1.0); // rotate the output frame with angle 90 degree
		Mat panaroma;
		cv::warpAffine(dst, panaroma, r, src.size());

		Rect Rec(0, 0, 1900, 500); //  undistorted region of interest selection
		rectangle(panaroma, Rec, Scalar(255), 1, 8, 0);
		Mat Roi = panaroma(Rec); 
		
		imshow("Rectangular_view", Roi);
		oVideoWriter.write(Roi); // save the interested region only
		
		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}
	destroyAllWindows();
	
	return 0;
}
