#include "visualer.hpp"

namespace svis {
	using namespace cv;

	Mat renderimg;

	void print(int y, int x, Vec3b color) {
			renderimg.at<cv::Vec3b>(y*4, x*4) = color;
			renderimg.at<cv::Vec3b>(y*4+1, x*4) = color;
			renderimg.at<cv::Vec3b>(y*4, x*4+1) = color;
			renderimg.at<cv::Vec3b>(y*4+1, x*4+1) = color;
			renderimg.at<cv::Vec3b>(y*4+2, x*4) = color;
			renderimg.at<cv::Vec3b>(y*4, x*4+2) = color;
			renderimg.at<cv::Vec3b>(y*4+2, x*4+2) = color;
			renderimg.at<cv::Vec3b>(y*4+3, x*4) = color;
			renderimg.at<cv::Vec3b>(y*4, x*4+3) = color;
			renderimg.at<cv::Vec3b>(y*4+3, x*4+3) = color;
			renderimg.at<cv::Vec3b>(y*4+1, x*4+2) = color;
			renderimg.at<cv::Vec3b>(y*4+1, x*4+3) = color;
			renderimg.at<cv::Vec3b>(y*4+2, x*4+1) = color;
			renderimg.at<cv::Vec3b>(y*4+2, x*4+3) = color;
			renderimg.at<cv::Vec3b>(y*4+3, x*4+1) = color;
			renderimg.at<cv::Vec3b>(y*4+3, x*4+2) = color;
		}

	void roi_drow(Mat frame1, Mat frame2) {
			Rect roi1(0, 240, 640, 4); // первый кусочек
			Rect roi2(0, 243, 640, 4); // второй кусочек
			Mat cropped1 = frame1(roi1); // вырезаем первый кусочек
			Mat cropped2 = frame2(roi2); // вырезаем второй кусочек
			Mat combined; // объединенное изображение
			vconcat(cropped1, cropped2, combined); // объединяем кусочки по горизонтали
			imshow("1", cropped1); // отображаем объединенное изображение
			imshow("2", cropped2); // отображаем объединенное изображение
	}

	void mono_drow(Mat frame1, std::string text) {
		cv::line(frame1, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(255, 0, 0), 1);
		cv::line(frame1, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(255, 0, 0), 1);

		cv::Scalar textColor(0, 0, 255); // цвет текста
		cv::putText(frame1, text, (cv::Point(10, 50)), cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor);

		imshow("Mono", frame1);
	}

	void so_drow(Mat frame1, std::string text) {
			cv::line(frame1, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(255, 0, 0), 1);
			cv::line(frame1, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(255, 0, 0), 1);
			cv::Scalar textColor(0, 0, 255); // цвет текста
			cv::putText(frame1, text, (cv::Point(10, 50)), cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor);

			imshow("Mono", frame1);
		}

	void stereo_drow(Mat frame1, Mat frame2) {
		cv::line(frame1, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(255, 0, 0), 1);
		cv::line(frame1, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(255, 0, 0), 1);

		cv::line(frame2, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(255, 0, 0), 1);
		cv::line(frame2, cv::Point(240, 0), cv::Point(240, 479), cv::Scalar(255, 0, 0), 1);

		namedWindow("Camera 1", cv::WINDOW_NORMAL);
		imshow("Camera 1", frame1);
		namedWindow("Camera 2", cv::WINDOW_NORMAL);
		imshow("Camera 2", frame2);
	}

	void depth_drow(unsigned char arr[120][160], std::string text1, std::string text2) {
		renderimg = Mat(Size(640*2, 480*2), CV_8UC3, Scalar(0, 0, 0));
		for(int x = 0; x < 160; x++)
			for(int y = 0; y < 120; y++) {
				if(arr[y][x] != 255) {
					if(arr[y][x] == 200) {
						print(y*2, x*2, {255, 0, 0});
					} else {
						uchar z = 3.1*arr[y][x];
						uchar red;
						uchar green=0;
						if(z > 128) {
							red = 178;
							green = 178 - (z-128);
						} else {
							green = 160;
							red = z+80;
						}

						print(y*2, x*2, {50, green, red});
					}
				}
			}
		cv::Scalar textColor(0, 0, 255); // цвет текста
		cv::putText(renderimg, text1, (cv::Point(10, 50)), cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor);
		cv::putText(renderimg, text2, (cv::Point(10, 100)), cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor);

		cv::line(renderimg, cv::Point(0, 240*2), cv::Point(639*2, 240*2), cv::Scalar(255, 0, 0), 1);
		cv::line(renderimg, cv::Point(320*2, 0), cv::Point(320*2, 479*2), cv::Scalar(255, 0, 0), 1);

		imshow("big", renderimg);
	}

}
