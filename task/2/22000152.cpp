#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	Mat gray_image = imread("lena.png", 0);
	Mat result = Mat::zeros(gray_image.rows, gray_image.rows, gray_image.type());
	unsigned char pix[256];
	float gamma;

	for (int i = 0; i < gray_image.rows; i++) {
		for (int j = 0; j < gray_image.cols; j++) {
			result.at<uchar>(j, i) = gray_image.at<uchar>(i, gray_image.cols-j-1);
		}
	}


	for (int i = 0; i < 256; i++) {	
		if (i < 127) {
			gamma = 0.5;
		}
		else {
			gamma = 10.0;
		}
		pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	}


	for (int j = 0; j < gray_image.rows; j++)
		for (int i = 0; i < gray_image.cols; i++)
			result.at<uchar>(j, i) = pix[result.at<uchar>(j, i)];


	imshow("result", result);

	imshow("gray image", gray_image);
	
	waitKey(0);
	return 0;
}