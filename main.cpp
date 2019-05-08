
#include "ReadVideo.h"
#include "PreProcess.h"
#include "LaneLineDetection.h"
#include <opencv2/opencv.hpp>

#if 1
int main() {
	PreProcess pre;
	LaneLineDetection laneDetection;
	// 先读取视频
	cv::VideoCapture cap;
	cap.open("D://My Code/LaneDetectionWithOpenCV/LaneDetection/test_videos/test.mp4");
	cv::Mat frame;
	// 进入循环
	while (1) {
		cap >> frame;
		if (frame.empty()) {
			break;
		}
		pre.PreProcessImage(frame);
		if (!pre.lines->empty()) {
			frame = laneDetection.LaneDetection(*pre.lines, frame, pre.img_edges);
			cv::imshow("Output", frame);
		}
		
		if (cv::waitKey(33) >= 0) {
			break;
		}

	}

	return 0;
}
#endif