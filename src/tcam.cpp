#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>

#include "framebuffer/FrameBuffer.hpp"
#include "stereo/stereo.hpp"
#include "stereo/so.hpp"
#include "vector/vector.hpp"

using namespace cv;
using namespace std;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
	cv::VideoCapture cap1(4); // открываем первую камеру
	if (!cap1.isOpened()) {
	    std::cerr << "Failed to open camera 1" << std::endl;
	    return -1;
	}

	cv::VideoCapture cap2(2); // открываем вторую камеру
	if (!cap2.isOpened()) {
	    std::cerr << "Failed to open camera 2" << std::endl;
	    return -1;
	}

	FrameBuffer buffer1(640, 480), buffer2(640 ,480);

	// поток чтения кадров из первой камеры
	std::thread thread1([&]() {
		cv::Mat frame_th;
		double timestamp;
	    while (cap1.read(frame_th)) {
	        timestamp = cap1.get(cv::CAP_PROP_POS_MSEC); // пв буфер
	        pthread_mutex_lock( &m1 );
	        buffer1.push(frame_th, timestamp);
	        pthread_mutex_unlock( &m1 );
	    }
	});

	// поток чтения кадров из второй камеры
	std::thread thread2([&]() {
		cv::Mat frame_th;
		double timestamp;
	    while (cap2.read(frame_th)) {
	        timestamp = cap2.get(cv::CAP_PROP_POS_MSEC); // получаем метку времени кадра из второй камеры
	        pthread_mutex_lock( &m2 );
			buffer2.push(frame_th, timestamp);
			pthread_mutex_unlock( &m2 );
	    }
	});

	cv::Mat frame1 = Mat(Size(640, 480), CV_8UC3, Scalar(0, 0, 255));
	cv::Mat frame2 = Mat(Size(640, 480), CV_8UC3, Scalar(0, 0, 255));

	BufferElement el1, el2;

	while (true) {
		pthread_mutex_lock( &m1 );
		pthread_mutex_lock( &m2 );
		el1 = buffer1.getLast();
		el2 = buffer2.getLast();

		if(el1.timestamp - el2.timestamp > 0) {
			el1 = buffer1.getNearest(el2.timestamp);
		} else {
			el2 = buffer2.getNearest(el1.timestamp);
		}

		memcpy(frame1.data, el1.frame.data, 640*480*3);
		memcpy(frame2.data, el2.frame.data, 640*480*3);

		pthread_mutex_unlock( &m1 );
		pthread_mutex_unlock( &m2 );

//		// рисование горизонтальной полосы
//		cv::line(frame1, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(0, 0, 0), 1);
//		// рисование вертикальной полосы
//		cv::line(frame1, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(0, 0, 0), 1);
//
//		// рисование горизонтальной полосы
//		cv::line(frame2, cv::Point(0, 240), cv::Point(639, 240), cv::Scalar(0, 0, 0), 1);
//		// рисование вертикальной полосы
//		cv::line(frame2, cv::Point(320, 0), cv::Point(320, 479), cv::Scalar(0, 0, 0), 1);
//
//		imshow("Camera 1", frame1);
//		imshow("Camera 2", frame2);
//		cv::waitKey(10);
		if(so::stereo_work(frame1, frame2)) {
			break;
		}
//		if(st::stereo_work(frame1, frame2)) {
//			break;
//		}
//		vt::vt_work(frame1);
	}

    destroyAllWindows();

    return 0;
}
