#include "PreProcess.h"

void PreProcess::PreProcessImage(cv::Mat image) {
	if (image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return;
	}

	// 对图像进行高斯模糊降噪
	img_denoise = PreProcess::GaussianBlurDeNise(image);
	// 再对图像进行边缘检测
	img_edges = PreProcess::EdgeDetector(img_denoise);
	// 再对图像进行掩膜计算
	img_mask = PreProcess::GetMask(img_edges);
	
	// 霍夫直线检测
	PreProcess::HoughLines(img_mask);
}

//// 切割图像
//cv::Mat PreProcess::CutImage(cv::Mat src_image) {
//	if (src_image.empty()) {
//		LoggerPreProcess.TraceWarn("Pre process cut image, image is empty");
//		return src_image;
//	}
//	int width  = src_image.rows;
//	int heigth = src_image.cols;
//	cv::Rect rect(0, width / 2, heigth, width / 2);
//	// cv::Rect rect(10, 100, 300, 100);
//	cv::Mat roi_image = src_image(rect);
//
//	return roi_image;
//}


// 对图像进行高斯模糊并降噪处理
cv::Mat PreProcess::GaussianBlurDeNise(cv::Mat src_image) {
	if (src_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return src_image;
	}
	cv::Mat output;
	cv::GaussianBlur(src_image, output, cv::Size(5, 5), 0, 0);
	return output;
}

// 对图像进行边缘检测
cv::Mat PreProcess::EdgeDetector(cv::Mat src_image) {
	if (src_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return src_image;
	}
	cv::Mat opt_image;
	cv::Mat cny_image;
	cv::Point anchor;

	// 图像灰度化
	cv::cvtColor(src_image, opt_image, cv::COLOR_RGB2GRAY);
	
	//cv::Canny(opt_image, cny_image, 50, 150, 3, true);

	// 图像二值化
	cv::threshold(opt_image, opt_image, 140, 255, cv::THRESH_BINARY);
	//cv::namedWindow("threshold", cv::WINDOW_AUTOSIZE);
	//cv::imshow("threshold", opt_image);

	/* anchor = cv::Point(-1, -1);
	kernel = cv::Mat(1, 3, CV_32F);
	kernel.at<float>(0, 0) = -1;
	kernel.at<float>(0, 1) = 0;
	kernel.at<float>(0, 2) = 1;

	cv::filter2D(opt_image, opt_image, -1, kernel, anchor, 0, cv::BORDER_DEFAULT); */

	cv::Canny(opt_image, cny_image, 50, 150, 3, true);


	// cv::imshow("Output", cny_image);
	return cny_image;
}

// 获取掩膜
/* 获取掩膜的思路：
** 设置图像中一个固定的区域
*/
cv::Mat PreProcess::GetMask(cv::Mat edge_image) {
	if (edge_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process edge image is empty!");
		return edge_image;
	}
	cv::Mat opt_image;
	// 构造一个大小为rows x cols，数据类型为type，值全为0的矩阵
	cv::Mat mask = cv::Mat::zeros(edge_image.size(), edge_image.type());
	//std::cout << "rows, cols: " << edge_image.size() << std::endl;
	//cv::Point pts[4] = {
	//	cv::Point(50, 230),
	//	cv::Point(50, 400),
	//	cv::Point(230, 400),
	//	cv::Point(230, 230),
	//};
	cv::Point pts[4] = {
		cv::Point(210, 720),
		cv::Point(550, 450),
		cv::Point(717, 450),
		cv::Point(1280, 720)
	};

	// 创建一个二元多边形的mask
	cv::fillConvexPoly(mask, pts, 4, cv::Scalar(25, 0, 0));
	// 把edge_image和mask相乘，得到输出的图片
	// bitwise_and对二进制数据进行“与”操作
	cv::bitwise_and(edge_image, mask, opt_image);

	return opt_image;

	///** 创建一些点 */

	//cv::Mat mask = cv::Mat::zeros(edge_image.size(), CV_8UC1);

	//cv::Point rook_points[1][4];
	//rook_points[0][0] = cv::Point(0, edge_image.rows);
	//rook_points[0][1] = cv::Point(460, 325);
	//rook_points[0][2] = cv::Point(520, 325);
	//rook_points[0][3] = cv::Point(edge_image.cols, edge_image.rows);

	//const cv::Point* ppt[1] = { rook_points[0] };
	//int npt[] = { 4 };

	//fillPoly(mask,
	//	ppt,
	//	npt,
	//	1,
	//	cv::Scalar(255, 255, 255), 8);

	//imshow("mask", mask);

	//cv::Mat dstImg;
	//edge_image.copyTo(dstImg, mask);

	//cv::imshow("dstImg", dstImg);

	//return dstImg;
}

// 霍夫直线变换
void PreProcess::HoughLines(cv::Mat dstImg) {
	std::vector<cv::Vec4i> line;

	/*
	** void HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold,double minLineLength=0, double maxLineGap=0 )
	** image : 边缘检测的输出图像.它应该是个灰度图(但事实上是个二值化图)
	** lines : 储存着检测到的直线的参数对的容器，也就是线段两个端点的坐标
	** rho  : 参数极径  以像素值为单位的分辨率.我们使用 1 像素.
	** theta : 参数极角  以弧度为单位的分辨率.我们使用 1度 (即CV_PI / 180)
	** threshold : 要”检测” 一条直线所需最少的的曲线交点
	** minLinLength : 能组成一条直线的最少点的数量.点数量不足的直线将被抛弃.线段的最小长度
	** maxLineGap : 线段上最近两点之间的阈值
	*/
	cv::HoughLinesP(dstImg, line, 1, CV_PI / 180, 20, 20, 20);
	*PreProcess::lines = line;
}