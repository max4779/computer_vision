#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void white_balacing(Mat img) {
	Mat bgr_channels[3];
	split(img, bgr_channels);
	double avg;
	int sum, temp, i, j, c;
	for (c = 0; c < img.channels(); c++) {
		sum = 0;
		avg = 0.0f;
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				sum += bgr_channels[c].at<uchar>(i, j);
			}
		}
		avg = sum / (img.rows * img.cols);
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);
				if (temp > 255) bgr_channels[c].at<uchar>(i, j) = 255;
				else bgr_channels[c].at<uchar>(i, j) = temp;
			}
		}
	}
	merge(bgr_channels, 3, img);
}

int main() {
	VideoCapture cap("video.mp4");
	Mat frame;
	bool neg = false;
	bool gam = false;
	bool his = false;
	bool sli = false;
	bool con = false;
	bool fil = false;
	bool uns = false;
	bool whi = false;
	uchar pix[256];
	int input = 0;
	vector<Mat> mo(3);
	uchar* h;
	uchar* s;
	Mat blurred;

	if (cap.open("video.mp4") == 0) {
		cout << "No file" << endl;
		return -1;
	}

	while (1) {
		cap >> frame;


		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}

		input = waitKey(10);

		if (neg) {
			frame = 255 - frame;
		}

		if (gam) {
			for (int i = 0; i < 256; i++) {
				pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), 2.5) * 255.0f);
			}
			for (int j = 0; j < frame.rows; j++) {
				for (int o = 0; o < frame.cols + 2560; o++) {
					frame.at<uchar>(j, o) = pix[frame.at<uchar>(j, o)];
				}
			}
		}

		if (his) {
			cvtColor(frame, frame, CV_BGR2GRAY);
			equalizeHist(frame, frame);
		}

		if (sli) {
			cvtColor(frame, frame, CV_BGR2HSV);
			split(frame, mo);

			for (int j = 0; j < frame.rows; j++) {
				h = mo[0].ptr<uchar>(j);
				s = mo[1].ptr<uchar>(j);
				for (int i = 0; i < frame.cols; i++) {
					if (h[i] > 9 && h[i] < 23) {
						s[i] = s[i];
					}
					else {
						s[i] = 0;
					}
				}
			}
			merge(mo, frame);
		}

		if (con) {
			cvtColor(frame, frame, CV_BGR2HSV);
			split(frame, mo);

			for (int j = 0; j < frame.rows; j++) {
				h = mo[0].ptr<uchar>(j);
				s = mo[1].ptr<uchar>(j);
				for (int i = 0; i < frame.cols; i++) {
					if (h[i] > 129) h[i] = h[i] - 129;
					else h[i] += 50;
				}
			}
			merge(mo, frame);
		}

		if (fil) {
			blur(frame, frame, Size(9, 9));
		}

		if (uns) {
			blur(frame, blurred, Size(9, 9));
			subtract(frame, blurred, frame);
		}

		if (whi) {
			white_balacing(frame);
		}

		if (input == 'n') {
			neg = true;
			gam = false;
			his = false;
			sli = false;
			con = false;
			fil = false;
			uns = false;
			whi = false;
		}

		else if (input == 'g') {
			neg = false;
			gam = true;
			his = false;
			sli = false;
			con = false;
			fil = false;
			uns = false;
			whi = false;
		}

		else if (input == 'h') {
			neg = false;
			gam = false;
			his = true;
			sli = false;
			con = false;
			fil = false;
			uns = false;
			whi = false;
		}
		else if (input == 's') {
			neg = false;
			gam = false;
			his = false;
			sli = true;
			con = false;
			fil = false;
			uns = false;
			whi = false;
		}
		else if (input == 'c') {
			neg = false;
			gam = false;
			his = false;
			sli = false;
			con = true;
			fil = false;
			uns = false;
			whi = false;
		}
		else if (input == 'a') {
			neg = false;
			gam = false;
			his = false;
			sli = false;
			con = false;
			fil = true;
			uns = false;
			whi = false;
		}
		else if (input == 'u') {
			neg = false;
			gam = false;
			his = false;
			sli = false;
			con = false;
			fil = false;
			uns = true;
			whi = false;
		}
		else if (input == 'w') {
			neg = false;
			gam = false;
			his = false;
			sli = false;
			con = false;
			fil = false;
			uns = false;
			whi = true;
		}
		else if (input == 'r') {
			neg = false;
			gam = false;
			his = false;
			sli = false;
			con = false;
			fil = false;
			uns = false;
			whi = false;
		}

		namedWindow("video");
		imshow("video", frame);

		if (input == 27) {
			break;
		}
	}

	cap.release();
	destroyAllWindows();
	return 0;
}
