#ifndef _PREPROCESS_H_
#define _PROPROCESS_H_

#include "Logger.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace LOGGER;

static CLogger LoggerPreProcess(CLogger::GetAppPathA().append("Log\\").append("PreProcessLog\\"));


// 对图片进行预处理的类
// 图片预处理的算法
/*
** ①图像灰度化
** ②图像高斯滤波
** ③边缘检测
** ④获取掩膜
** ⑤对图像进行霍夫直线检测
** ④把处理好的图像返回
*/
class PreProcess {
public:
	PreProcess() {
		lines = new std::vector<cv::Vec4i>();
	};
	~PreProcess() {
		if (lines != NULL) {
			delete lines;
		}
	};
	// 为了封装性，只留一个接口，即返回预处理后的图片
	void PreProcessImage(cv::Mat image);
// private:
	// cv::Mat CutImage(cv::Mat src_image);
	cv::Mat GaussianBlurDeNise(cv::Mat src_image);
	cv::Mat EdgeDetector(cv::Mat src_image);
	cv::Mat GetMask(cv::Mat edge_image);
	void HoughLines(cv::Mat dstImg);
public:
	std::vector<cv::Vec4i>* lines;
	cv::Mat img_denoise;
	cv::Mat img_edges;
	cv::Mat img_mask;
};

#endif // !_PREPROCESS_H_
