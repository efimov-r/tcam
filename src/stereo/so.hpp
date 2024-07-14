#ifndef STEREO_SO_HPP_
#define STEREO_SO_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <math.h>

namespace so {
	bool stereo_work(cv::Mat& frame1, cv::Mat& frame2);
}



#endif /* STEREO_SO_HPP_ */
