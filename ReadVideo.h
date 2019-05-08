#ifndef _READVIDEO_H_
#define _READVIDEO_H_

#include"Logger.h"
#include <opencv2/opencv.hpp>

using namespace LOGGER;

static CLogger LoggerReadVideo(CLogger::GetAppPathA().append("Log\\").append("ReadLog\\"));

// ��ȡʵʱ��Ƶ����
class ReadVideo {
public:
	ReadVideo(int cameraNumber = 0);
	~ReadVideo();
	// ����һ��VideoCapture���͵Ķ���
	cv::VideoCapture* readVideoCap();
private:
	cv::VideoCapture* _cap;
};

#endif // _READVIDEO_H_

