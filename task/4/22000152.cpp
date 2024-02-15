#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	VideoCapture cap;
	Mat right, right_gray, right_canny, left, left_gray, left_canny, frame;
	

	if (cap.open("Road.mp4") == 0) {
		cout << "No file" << endl;
		waitKey(0);
	}

	Rect left_roi(200,400,400,200);
	Rect right_roi(600, 400, 400, 200);
	bool stop = true;

	int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
	double fps = cap.get(CAP_PROP_FPS);
	int delay = total_frames / fps;
	double limit_time;

	vector<Vec2f> left_line, f_left_line, right_line, f_right_line;
	float left_rho, left_theta, right_rho, right_theta, left_a, left_b, left_x0, left_y0, right_a, right_b, right_x0, right_y0;
	Point left_p1, left_p2, right_p1, right_p2;

	while (stop) {
		cap >> frame;

		right = frame(right_roi);
		left = frame(left_roi);

		cvtColor(right, right_gray, CV_BGR2GRAY);
		cvtColor(left, left_gray, CV_BGR2GRAY);

		blur(right_gray, right_gray, Size(5, 5));
		blur(left_gray, left_gray, Size(5, 5));

		Canny(right_gray, right_canny, 10, 60 );
		Canny(left_gray, left_canny, 10, 60);

		HoughLines(left_canny,left_line,1, CV_PI / 180, 100);
		HoughLines(right_canny, right_line, 1, CV_PI / 180, 100);

		float ALR=0 , ALT=0, ARR=0, ART=0;

		for (int i = 0; i < left_line.size(); i++) {
			left_rho = left_line[i][0];
			left_theta = left_line[i][1];

			if (left_theta * 180 / CV_PI >= 30 && left_theta * 180 / CV_PI <= 60) {
				f_left_line.push_back(left_line[i]);
			}
		}

		for (int i = 0; i < right_line.size(); i++) {
			right_rho = right_line[i][0];
			right_theta = right_line[i][1];
			
			if (right_theta*180/CV_PI >= 120 && right_theta* 180 / CV_PI <= 150) {
				f_right_line.push_back(right_line[i]);
			}
		}
		
		for (int i = 0; i < f_left_line.size(); i++) {
			ALR += f_left_line[i][0];
			ALT += f_left_line[i][1];
		}

		for (int i = 0; i < f_right_line.size(); i++) {
			ARR += f_right_line[i][0];
			ART += f_right_line[i][1];
		}

		if (!f_left_line.empty()) {
			ALR = ALR / f_left_line.size();
			ALT = ALT / f_left_line.size();
			left_a = cos(ALT);
			left_b = sin(ALT);
			left_x0 = left_a * ALR;
			left_y0 = left_b * ALR;
			left_p1 = Point(cvRound(left_x0 + 1000 * (-left_b)), cvRound(left_y0 + 1000 * left_a));
			left_p2 = Point(cvRound(left_x0 - 1000 * (-left_b)), cvRound(left_y0 - 1000 * left_a));
			line(frame, left_p1, left_p2, Scalar(0, 0, 255), 3, LINE_AA);
		}


		if (!f_right_line.empty()) {
			ARR = ARR / f_right_line.size();
			ART = ART / f_right_line.size();
			right_a = cos(ART);
			right_b = sin(ART);
			right_x0 = right_a * ARR;
			right_y0 = right_b * ARR;
			right_p1 = Point(cvRound(right_x0 + 1000 * (-right_b)), cvRound(right_y0 + 1000 * right_a));
			right_p2 = Point(cvRound(right_x0 - 1000 * (-right_b)), cvRound(right_y0 - 1000 * right_a));
			line(frame, right_p1, right_p2, Scalar(0, 0, 255), 3, 8);
		}
		
		imshow("aa", frame);
		namedWindow("Left canny");
		namedWindow("Right canny");
		moveWindow("Left canny",200, 0);
		moveWindow("Right canny",600, 0);
		imshow("Left canny", left_canny);
		imshow("Right canny", right_canny);
		waitKey(delay);

		double limit_time = cap.get(CAP_PROP_POS_FRAMES);
		if (limit_time > 175) {
			stop = false;
		}
	}

}
