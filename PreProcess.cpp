#include "PreProcess.h"

void PreProcess::PreProcessImage(cv::Mat image) {
	if (image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return;
	}

	// ��ͼ����и�˹ģ������
	img_denoise = PreProcess::GaussianBlurDeNise(image);
	// �ٶ�ͼ����б�Ե���
	img_edges = PreProcess::EdgeDetector(img_denoise);
	// �ٶ�ͼ�������Ĥ����
	img_mask = PreProcess::GetMask(img_edges);
	
	// ����ֱ�߼��
	PreProcess::HoughLines(img_mask);
}

//// �и�ͼ��
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


// ��ͼ����и�˹ģ�������봦��
cv::Mat PreProcess::GaussianBlurDeNise(cv::Mat src_image) {
	if (src_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return src_image;
	}
	cv::Mat output;
	cv::GaussianBlur(src_image, output, cv::Size(5, 5), 0, 0);
	return output;
}

// ��ͼ����б�Ե���
cv::Mat PreProcess::EdgeDetector(cv::Mat src_image) {
	if (src_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process image is empty!");
		return src_image;
	}
	cv::Mat opt_image;
	cv::Mat cny_image;
	cv::Point anchor;

	// ͼ��ҶȻ�
	cv::cvtColor(src_image, opt_image, cv::COLOR_RGB2GRAY);
	
	//cv::Canny(opt_image, cny_image, 50, 150, 3, true);

	// ͼ���ֵ��
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

// ��ȡ��Ĥ
/* ��ȡ��Ĥ��˼·��
** ����ͼ����һ���̶�������
*/
cv::Mat PreProcess::GetMask(cv::Mat edge_image) {
	if (edge_image.empty()) {
		LoggerPreProcess.TraceWarn("Pre process edge image is empty!");
		return edge_image;
	}
	cv::Mat opt_image;
	// ����һ����СΪrows x cols����������Ϊtype��ֵȫΪ0�ľ���
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

	// ����һ����Ԫ����ε�mask
	cv::fillConvexPoly(mask, pts, 4, cv::Scalar(25, 0, 0));
	// ��edge_image��mask��ˣ��õ������ͼƬ
	// bitwise_and�Զ��������ݽ��С��롱����
	cv::bitwise_and(edge_image, mask, opt_image);

	return opt_image;

	///** ����һЩ�� */

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

// ����ֱ�߱任
void PreProcess::HoughLines(cv::Mat dstImg) {
	std::vector<cv::Vec4i> line;

	/*
	** void HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold,double minLineLength=0, double maxLineGap=0 )
	** image : ��Ե�������ͼ��.��Ӧ���Ǹ��Ҷ�ͼ(����ʵ���Ǹ���ֵ��ͼ)
	** lines : �����ż�⵽��ֱ�ߵĲ����Ե�������Ҳ�����߶������˵������
	** rho  : ��������  ������ֵΪ��λ�ķֱ���.����ʹ�� 1 ����.
	** theta : ��������  �Ի���Ϊ��λ�ķֱ���.����ʹ�� 1�� (��CV_PI / 180)
	** threshold : Ҫ����⡱ һ��ֱ���������ٵĵ����߽���
	** minLinLength : �����һ��ֱ�ߵ����ٵ������.�����������ֱ�߽�������.�߶ε���С����
	** maxLineGap : �߶����������֮�����ֵ
	*/
	cv::HoughLinesP(dstImg, line, 1, CV_PI / 180, 20, 20, 20);
	*PreProcess::lines = line;
}