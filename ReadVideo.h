#ifndef _READVIDEO_H_
#define _READVIDEO_H_

#include"Logger.h"
#include <opencv2/opencv.hpp>

using namespace LOGGER;

static CLogger LoggerReadVideo(CLogger::GetAppPathA().append("Log\\").append("ReadLog\\"));

// 读取实时视频的类
class ReadVideo {
public:
	ReadVideo(int cameraNumber = 0);
	~ReadVideo();
	// 返回一个VideoCapture类型的对象
	cv::VideoCapture* readVideoCap();
private:
	cv::VideoCapture* _cap;
};

#endif // _READVIDEO_H_

