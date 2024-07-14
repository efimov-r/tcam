#ifndef STEREO_STEREO_HPP_
#define STEREO_STEREO_HPP_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

namespace st {
	bool stereo_work(cv::Mat frame1, cv::Mat frame2);
}

#endif /* STEREO_STEREO_HPP_ */
