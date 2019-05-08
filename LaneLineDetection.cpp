#include "LaneLineDetection.h"

// ����ֻ�ṩһ���ӿ�
cv::Mat LaneLineDetection::LaneDetection(std::vector<cv::Vec4i> lines, cv::Mat frame, cv::Mat edge_image) {
	cv::Mat output;
	if (lines.empty() || frame.empty() || edge_image.empty()) {
		return output;
	}
	// ��lines��Ϊ��������
	LRLines = LineSeparation(lines, edge_image);
	// ���ÿ�೵����һ����
	lane = Regression(LRLines, frame);
	// ת��Ԥ��
	turn = PredictTurn();

	// ��������������
	output = PlotLane(frame, lane, turn);
	
	return output;
}


// �Լ�⵽���߶Σ�����б�ʷ�Ϊ��������ֱ��
std::vector<std::vector<cv::Vec4i>> LaneLineDetection::LineSeparation(std::vector<cv::Vec4i> lines, cv::Mat edge_image) {
	std::vector<std::vector<cv::Vec4i>> opt(2);
	if (lines.empty() || edge_image.empty()) {
		LoggerLaneLineDetection.TraceWarn("Lane line detection lines is empty, or edge_image is empty!");
		return opt;
	}
	size_t j = 0;
	cv::Point ini, fini;
	
	double slope_thresh = 0.3;

	std::vector<double> slopes;
	std::vector<cv::Vec4i> selected_lines;
	std::vector<cv::Vec4i> right_lines, left_lines;

	
	// �������м�⵽�ĳ����ߵ�б��
	for (auto i : lines) {
		ini = cv::Point(i[0], i[1]);
		fini = cv::Point(i[2], i[3]);

		double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y)) / (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);

		if (std::abs(slope) > slope_thresh) {
			slopes.push_back(slope);
			selected_lines.push_back(i);
		}
	}

	// �����еĳ����߷�Ϊ��������
	cnt_image = static_cast<double>((edge_image.cols / 2));
	while (j < selected_lines.size()) {
		ini = cv::Point(selected_lines[j][0], selected_lines[j][1]);
		fini = cv::Point(selected_lines[j][2], selected_lines[j][3]);

		// �ѳ����߷�Ϊ��ߺ��ұ�����
		if (slopes[j] > 0 && fini.x > cnt_image && ini.x > cnt_image) {
			right_lines.push_back(selected_lines[j]);
			right_flag = true;
		}
		else if (slopes[j] < 0 && fini.x < cnt_image && ini.x < cnt_image) {
			left_lines.push_back(selected_lines[j]);
			left_flag = true;
		}
		j++;
	}

	opt[0] = right_lines;
	opt[1] = left_lines;

	return opt;
}

// ÿ������ÿ��ֻ��һ����
std::vector<cv::Point> LaneLineDetection::Regression(std::vector<std::vector<cv::Vec4i>> left_right_lines, cv::Mat ipt_Image) {
	std::vector<cv::Point> output(4);
	cv::Point ini;
	cv::Point fini;
	cv::Point ini2;
	cv::Point fini2;
	cv::Vec4d right_line;
	cv::Vec4d left_line;
	std::vector<cv::Point> right_pts;
	std::vector<cv::Point> left_pts;

	// �����⵽���ұߵĳ����ߣ�ʹ�ü�⵽�ĳ�����ֱ�ߵ��������յ�����һ��ֱ��
	if (right_flag == true) {
		for (auto i : left_right_lines[0]) {
			ini = cv::Point(i[0], i[1]);
			fini = cv::Point(i[2], i[3]);

			right_pts.push_back(ini);
			right_pts.push_back(fini);
		}

		if (right_pts.size() > 0) {
			// �ҳ����������form
			cv::fitLine(right_pts, right_line, cv::DIST_L2, 0, 1e-2, 1e-2);
			right_m = right_line[1] / right_line[0];
			right_b = cv::Point(right_line[2], right_line[3]);
		}
	}

	// ���������ߵ�ֱ��
	if (left_flag == true) {
		for (auto j : left_right_lines[1]) {
			ini2 = cv::Point(j[0], j[1]);
			fini2 = cv::Point(j[2], j[3]);

			left_pts.push_back(ini2);
			left_pts.push_back(fini2);
		}

		if (left_pts.size() > 0) {
			// The left line is formed here
			cv::fitLine(left_pts, left_line, cv::DIST_L2, 0, 1e-2, 1e-2);
			left_m = left_line[1] / left_line[0];
			left_b = cv::Point(left_line[2], left_line[3]);
		}
	}

	// �����б�ʺ�ƫ�Ƶ㣬Ӧ��ֱ�߷������ֱ�ߵ�
	int ini_y = ipt_Image.rows;
	int fin_y = 460;

	double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
	double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

	double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
	double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;

	output[0] = cv::Point(right_ini_x, ini_y);
	output[1] = cv::Point(right_fin_x, fin_y);
	output[2] = cv::Point(left_ini_x, ini_y);
	output[3] = cv::Point(left_fin_x, fin_y);

	return output;
}

// ת��Ԥ��
std::string LaneLineDetection::PredictTurn() {
	std::string output;
	double vanish_x;
	double thr_vp = 10;

	// ��ʧ�ĵ��������������ߵ��ཻ��
	vanish_x = static_cast<double>(((right_m*right_b.x) - (left_m*left_b.x) - right_b.y + left_b.y) / (right_m - left_m));

	// ��ʧ������˳�Ҫ��Ҫת��
	if (vanish_x < (cnt_image - thr_vp))
		output = "Left Turn";
	else if (vanish_x > (cnt_image + thr_vp))
		output = "Right Turn";
	else if (vanish_x >= (cnt_image - thr_vp) && vanish_x <= (cnt_image + thr_vp))
		output = "Straight";

	return output;
}

// ��ͼ���
cv::Mat LaneLineDetection::PlotLane(cv::Mat inputImage, std::vector<cv::Point> lane, std::string turn) {
	std::vector<cv::Point> poly_points;
	cv::Mat output;

	// ����һ��͸���Ķ���Σ����Ը��õĿ��ӻ�����
	inputImage.copyTo(output);
	poly_points.push_back(lane[2]);
	poly_points.push_back(lane[0]);
	poly_points.push_back(lane[1]);
	poly_points.push_back(lane[3]);
	cv::fillConvexPoly(output, poly_points, cv::Scalar(100, 100, 10), 16, 0);
	cv::addWeighted(output, 0.3, inputImage, 1.0 - 0.3, 0, inputImage);

	// ���Ƴ����߽��������
	// cv::line(inputImage, lane[0], lane[1], cv::Scalar(0, 255, 255), 8, 16);
	// cv::line(inputImage, lane[2], lane[3], cv::Scalar(0, 255, 255), 8, 16);

	// ����ת����Ϣ
	cv::putText(inputImage, turn, cv::Point(50, 90), cv::FONT_HERSHEY_COMPLEX_SMALL, 3, cv::Scalar(0, 255, 0), 1, 16);
	
	return inputImage;
}