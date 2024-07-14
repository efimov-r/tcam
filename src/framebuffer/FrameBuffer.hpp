#ifndef FRAMEBUFFER_FRAMEBUFFER_HPP_
#define FRAMEBUFFER_FRAMEBUFFER_HPP_

#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>

#define BUFF_SIZE 50 // сколько элементов в одном буфере

struct BufferElement {
	cv::Mat frame;
	double timestamp;
};

class FrameBuffer {
public:
	FrameBuffer(int w, int h) : max_size_(BUFF_SIZE) {
		const BufferElement& emptyEl = {cv::Mat(cv::Size(w, h), CV_8UC3, cv::Scalar(0, 0, 255)), 0};
		buffer_.push_back(emptyEl);
	}

	void push(const cv::Mat& frame, double timestamp);

	const BufferElement& getNearest(double timestamp);

	const BufferElement& getLast();

private:
	std::vector<BufferElement> buffer_;
	int max_size_;
};

#endif /* FRAMEBUFFER_FRAMEBUFFER_HPP_ */
