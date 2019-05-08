#ifndef LANELINEDETECTION_H_
#define LANELINEDETECTION_H_

#include "Logger.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace LOGGER;

static CLogger LoggerLaneLineDetection(CLogger::GetAppPathA().append("Log\\").append("LaneLineDetectiontLog\\"));

class LaneLineDetection {
public:
	LaneLineDetection() {};
	~LaneLineDetection() {};
	cv::Mat LaneDetection(std::vector<cv::Vec4i> lines, cv::Mat frame, cv::Mat edge_image);
private:
	// 对检测到的线段，按其斜率分为左右两种直线
	std::vector<std::vector<cv::Vec4i>> LineSeparation(std::vector<cv::Vec4i> lines, cv::Mat edge_image);
	// 每条车道每边只有一条线
	std::vector<cv::Point> Regression(std::vector<std::vector<cv::Vec4i>> left_right_lines, cv::Mat ipt_Image);
	// 转弯预测
	std::string PredictTurn();
	// 制图结果
	cv::Mat PlotLane(cv::Mat inputImage, std::vector<cv::Point> lane, std::string turn);

private:
	double size_image;
	double cnt_image;
	bool left_flag = false;  // Tells us if there's left boundary of lane detected
	bool right_flag = false;  // Tells us if there's right boundary of lane detected
	cv::Point right_b;  // Members of both line equations of the lane boundaries:
	double right_m;  // y = m*x + b
	cv::Point left_b;  
	double left_m;  
	std::vector<std::vector<cv::Vec4i> > LRLines;
	std::vector<cv::Point> lane;
	std::string turn;
};

#endif // !LANELINEDETECTION_H_

