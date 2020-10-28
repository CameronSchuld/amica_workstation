#pragma once

#include <iostream>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/tracking.hpp>

#define ARG_KNN 1001
#define ARG_MOG2 1002

#define ARG_LENGTH 1003
#define ARG_POWER 1004
#define ARG_SHADOW 1005
#define ARG_NOSHADOW 1006


class vision
{
public:
	//vision(cv::Mat frame);

	bool recordFrame(cv::Mat&);
	bool blurFrame(cv::Mat&, int blurX = 5, int blurY = 5);

	bool subtractBackground(cv::Mat, int bg_sub_type = ARG_KNN, int bg_length = 200, int bg_power = 200, int bg_shadow = ARG_SHADOW);
	bool trackObject(cv::VideoCapture&);
	bool contourObject(cv::VideoCapture&);
	bool createMask(cv::VideoCapture&);
	bool threshold(cv::VideoCapture&);
	bool dilate(cv::Mat&);
	bool erode(cv::Mat&);
	bool canny(cv::VideoCapture&);
	bool hull(cv::VideoCapture&);

private:

	cv::Mat unalteredFrame;
	cv::Mat alteredFrame;
	cv::Ptr<cv::BackgroundSubtractor> pBackSub;

};