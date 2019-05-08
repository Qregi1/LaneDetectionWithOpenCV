///** 测试日志和读取视频的类
//#include "ReadVideo.h"
//
//
//int main() {
//	ReadVideo readVideo;
//	// 创建一个窗口
//	namedWindow("testReadVideo", WINDOW_AUTOSIZE);
//
//	VideoCapture* cap =  readVideo.readVideoCap();
//	
//	if (!(*cap).isOpened()) {
//		logger.TraceError("Couldn't open capture");
//		return -1;
//	}
//
//	Mat frame;
//	char keyCode;
//	while (1) {
//		*cap >> frame;
//		if (frame.empty()) {
//			break;
//		}
//		imshow("testReadVideo", frame);
//		keyCode = waitKey(30);
//		if (keyCode == 27) {
//			break;
//		}
//	}
//
//	return -1;
//}
//*/
//
#if 0
#include "PreProcess.h"

int main() {
	cv::Mat ipt_imagr = cv::imread("D://My Code/LaneDetectionWithOpenCV/lane.jpg", -1);
	PreProcess pre;

	//cv::namedWindow("test1", cv::WINDOW_AUTOSIZE);
	//ipt_imagr = pre.CutImage(ipt_imagr);
	//ipt_imagr =  pre.GaussianBlurDeNise(ipt_imagr);
	//cv::imshow("test1", ipt_imagr);

	//cv::namedWindow("test2", cv::WINDOW_AUTOSIZE);
	//ipt_imagr = pre.EdgeDetector(ipt_imagr);
	//cv::imshow("test2", ipt_imagr);

	cv::namedWindow("test1", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("test2", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("test3", cv::WINDOW_AUTOSIZE);


	cv::imshow("test1", ipt_imagr);
	ipt_imagr = pre.GaussianBlurDeNise(ipt_imagr);
	ipt_imagr = pre.EdgeDetector(ipt_imagr);
	cv::imshow("test2", ipt_imagr);

	ipt_imagr = pre.GetMask(ipt_imagr);
	cv::imshow("test3", ipt_imagr);
	cv::waitKey(0);

	
	//cv::namedWindow("ReadVideo", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
	
	//// 读取视频，并对视频进行播放
	//cv::VideoCapture cap;
	//cap.open("D://My Code/LaneDetectionWithOpenCV/LaneDetection/test_videos/test.mp4");
	//cv::Mat frame;
	//PreProcess pre;
	//while (1) {
	//	cap >> frame;
	//	if (frame.empty()) {
	//		break;
	//	}
	//	// cv::imshow("ReadVideo", frame);

	//	// pre.PreProcessImage(frame);
	//	// cv::imshow("Video", frame);

	//	if (cv::imwrite("test.jpg", frame)) {

	//	}
	//	

	//	if (cv::waitKey(33) >= 0) {
	//		break;
	//	}
	//}
	//
	

	return 0;
}

#endif