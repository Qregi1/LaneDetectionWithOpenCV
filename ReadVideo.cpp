#include "ReadVideo.h"

ReadVideo::ReadVideo(int cameraNumber) {
	// 默认打开的摄像头为0号
	cv::VideoCapture* cap = new cv::VideoCapture();
	ReadVideo::_cap = cap;
	if (_cap != NULL) {
		LoggerReadVideo.TraceInfo("Read video open camera number is %d", cameraNumber);
		(*_cap).open(cameraNumber);
	}
	
}

cv::VideoCapture* ReadVideo::readVideoCap() {
	if (_cap != NULL) {
		return _cap;
	}
	LoggerReadVideo.TraceError("Read video the VideoCapture is empty!");
	return NULL;
}

ReadVideo::~ReadVideo() {
	if (_cap != NULL) {
		(*_cap).release();
		delete _cap;
	}
	LoggerReadVideo.TraceWarn("Read video try to delete a emtpy capture!");
}