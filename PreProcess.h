#ifndef _PREPROCESS_H_
#define _PROPROCESS_H_

#include "Logger.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace LOGGER;

static CLogger LoggerPreProcess(CLogger::GetAppPathA().append("Log\\").append("PreProcessLog\\"));


// ��ͼƬ����Ԥ�������
// ͼƬԤ������㷨
/*
** ��ͼ��ҶȻ�
** ��ͼ���˹�˲�
** �۱�Ե���
** �ܻ�ȡ��Ĥ
** �ݶ�ͼ����л���ֱ�߼��
** �ܰѴ���õ�ͼ�񷵻�
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
	// Ϊ�˷�װ�ԣ�ֻ��һ���ӿڣ�������Ԥ������ͼƬ
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
