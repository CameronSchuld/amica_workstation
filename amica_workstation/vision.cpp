#include "vision.h"

//Call when next frame is available
bool vision::nextFrame(cv::Mat newFrame)
{
	newFrame.copyTo(unalteredFrame);
	newFrame.copyTo(alteredFrame);

	return true;
}

//Not sure what this is used for tbh
bool vision::recordFrame(cv::Mat& image)
{
	cv::Mat differenceFrame;

	return false;
}

//Blurs whatever mat image is in the class
bool vision::blurFrame(int blurX, int blurY)
{
	std::cout << "Blur: (" << blurX << ", " << blurY << ")\n";
	cv::blur(alteredFrame, alteredFrame, cv::Point(blurX, blurY));

	return false;
}

//Initializes background subtraction with parameters (should only be called once per video, not once per frame)
bool vision::initBackgroundSeparation(int bg_sub_type, int bg_length, int bg_power, int bg_shadow)
{
	std::cout << "Background Subtraction: (" << bg_sub_type << ", " << bg_length << ", "
		<< bg_power << ", " << bg_shadow << ")\n";

	cv::Mat fgMask(alteredFrame.size(), CV_8UC1, cv::Scalar(0, 0, 0));

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

	return true;
}

//Applies background subtraction on mat image
bool vision::subtractBackground()
{
	pBackSub->apply(alteredFrame, fgMask);

	return true;
}

//Creates threshold image
bool vision::thresh(int thresh_value, int max_thresh_value)
{
	cv::threshold(fgMask, threshold, thresh_value, max_thresh_value, cv::THRESH_BINARY);
	return true;
}

//Dilates image after thresholding
bool vision::dilate()
{
	cv::dilate(threshold, threshold, structuringElement);
	return true;
}

//Erodes image after thresholding
bool vision::erode()
{
	cv::erode(threshold, threshold, structuringElement);
	return true;
}

//Canny threshold image
bool vision::canny(int threshold1, int threshold2)
{
	cv::Canny(threshold, canny_output, threshold1, threshold2);
	return true;
}

//Contour Object
bool vision::contourObject()
{
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	return true;
}

//Draws Contours to Mat Image
bool vision::drawContours()
{
	drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::drawContours(drawing, contours, (int)i, red, 2, cv::LINE_8, hierarchy, 0);
	}

	return true;
}

//Subtracts foreground
bool vision::subtractForeground()
{
	pBackSub->apply(unalteredFrame, bgMask);
	return true;
}