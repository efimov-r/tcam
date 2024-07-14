#ifndef STEREO_VISUALER_HPP_
#define STEREO_VISUALER_HPP_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

namespace svis {
	void roi_drow(cv::Mat frame1, cv::Mat frame2);
	void mono_drow(cv::Mat frame1, std::string text);
	void so_drow(cv::Mat frame1, std::string text);
	void stereo_drow(cv::Mat frame1, cv::Mat frame2);
	void depth_drow(unsigned char arr[120][160], std::string text1, std::string text2);
}

#endif
