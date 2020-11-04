#pragma once

#include <iostream>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/tracking.hpp>

//Argument list for background subtraction
#define ARG_KNN 1001
#define ARG_MOG2 1002

#define ARG_LENGTH 1003
#define ARG_POWER 1004
#define ARG_SHADOW 1005
#define ARG_NOSHADOW 1006

//Unique identifiers for all augmentation functionality
#define AUGMENTATION_BACKGROUND_SUBTRACTION 3001
#define AUGMENTATION_FOREGROUND_SUBTRACTION 3002
#define AUGMENTATION_BLUR 3003
#define AUGMENTATION_THRESHOLD 3004
#define AUGMENTATION_DILATE 3005
#define AUGMENTATION_ERODE 3006
#define AUGMENTATION_CANNY 3007
#define AUGMENTATION_CONTOUR 3008
#define AUGMENTATION_TRACK 3009
#define AUGMENTATION_MASK 3010


class vision
{
public:
	//System Functions
	bool nextFrame(cv::Mat);
	bool recordFrame(cv::Mat&);

	//Setup Augment Functions
	bool initBackgroundSeparation(int bg_sub_type = ARG_KNN, int bg_length = 200, int bg_power = 200, int bg_shadow = ARG_SHADOW);


	//Augment Functions
	bool subtractBackground();
	bool blurFrame(int blurX = 5, int blurY = 5);

	//Background Sub Functions
	bool thresh(int thresh_value = 200, int max_thresh_value = 255);
	bool contourObject();

	//Thresholding Functions
	bool dilate();
	bool erode();
	bool canny(int threshold1 = 5, int threshold2 = 5);

	//Foreground Sub Functions
	bool subtractForeground();

	//Special Functions
	bool trackObject(cv::VideoCapture&);
	bool createMask(cv::VideoCapture&);

	//Exporting
	bool drawContours();
	


	cv::Mat unalteredFrame;
	cv::Mat alteredFrame;
	cv::Mat drawing;

private:
	//Background Subtraction
	cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	cv::Mat fgMask;

	//Foreground Subtraction
	cv::Mat bgMask;

	//Thresholding
	cv::Mat threshold;
	cv::Mat canny_output;
	
	//Countouring
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> contours2;
	std::vector<cv::Vec4i> hierarchy;

	//Hulling

	//Object Tracking

	//Dilating and Eroding
	cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));


	//Colors
	cv::Scalar red = cv::Scalar(0, 0, 255);
};