#include "../framebuffer/FrameBuffer.hpp"


void FrameBuffer::push(const cv::Mat& frame, double timestamp) {
	if((int)buffer_.size() == max_size_) {
		buffer_.erase(buffer_.begin());
	}
	buffer_.push_back({frame.clone(), timestamp});
}

const BufferElement& FrameBuffer::getNearest(double timestamp){
	auto it = std::lower_bound(buffer_.begin(), buffer_.end(), timestamp,
		[](const BufferElement& elem, double ts) { return elem.timestamp < ts; });
	if (it == buffer_.end()) {
		return buffer_.back(); // если все элементы меньше заданного timestamp, то возвращаем последний элемент
	} else if (it == buffer_.begin()) {
		return buffer_.front(); // если все элементы больше заданного timestamp, то возвращаем первый элемент
	} else {
		double diff1 = std::abs((it-1)->timestamp - timestamp);
		double diff2 = std::abs(it->timestamp - timestamp);
		return diff1 < diff2 ? *(it-1) : *it; // выбираем элемент с ближайшим timestamp
	}
}

const BufferElement& FrameBuffer::getLast(){
	return buffer_.back();
}

