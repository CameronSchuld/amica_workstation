#include "vision.h"


//vision::vision(cv::Mat frame)
//{
//	frame.copyTo(alteredFrame);
//	frame.copyTo(unalteredFrame);
//
//	return;
//}

bool vision::recordFrame(cv::Mat& image)
{
	cv::Mat differenceFrame;

	return false;
}

bool vision::blurFrame(cv::Mat& image, int blurX, int blurY)
{
	std::cout << "Blur: (" << blurX << ", " << blurY << ")\n";
	cv::blur(image, image, cv::Point(blurX, blurY));

	return false;
}

bool vision::subtractBackground(cv::Mat frame, int bg_sub_type, int bg_length, int bg_power, int bg_shadow)
{
	cv::Mat fgMask(unalteredFrame.size(), CV_8UC1, cv::Scalar(0, 0, 0));

	int intArg1;
	int intArg2;
	int intArg3;

	bool boolArg = false;

	if (bg_shadow == ARG_SHADOW)
		bool boolArg = true;
	else
		bool boolArg = false;

	if (bg_sub_type == ARG_KNN)
		pBackSub = cv::createBackgroundSubtractorKNN(bg_length, bg_power, boolArg);

	if (bg_sub_type == ARG_MOG2)
		pBackSub = cv::createBackgroundSubtractorMOG2(bg_length, bg_power, boolArg);

	pBackSub->apply(frame, fgMask);

	fgMask.copyTo(alteredFrame);

	return true;
}