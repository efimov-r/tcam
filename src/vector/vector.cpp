#include "vector.hpp"

namespace vt {
	using namespace cv;

	Mat hsv1;
	Mat grame1;

	void mono_drow(Mat frame1, std::string text) {
		cv::line(frame1, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(255, 0, 0), 1);
		cv::line(frame1, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(255, 0, 0), 1);

		cv::Scalar textColor(0, 0, 255); // цвет текста
		cv::putText(frame1, text, (cv::Point(10, 50)), cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor);

		imshow("Mono", frame1);
		cv::waitKey(10);
	}


	void vt_work(Mat f1){
		int i = 3;
		cvtColor(f1, hsv1, COLOR_BGR2HSV);


		mono_drow(hsv1, "Mono");



		std::cout << i << std::endl;
	}
}
