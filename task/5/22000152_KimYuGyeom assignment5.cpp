#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	Mat FP_origin = imread("finger_print.png",0);
	Mat adp = imread("adaptive.png",0);
	Mat adp_1 = imread("adaptive_1.jpg",0);
	Mat finger_print, adaptive, adaptive_1;
	
	threshold(FP_origin, finger_print, 0, 255, THRESH_BINARY | THRESH_OTSU);
	adaptiveThreshold(adp, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
	adaptiveThreshold(adp_1, adaptive_1, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 10);

	namedWindow("finger_print");
	namedWindow("adaptive");
	namedWindow("adaptive_1");

	imshow("finger_print", finger_print);
	imshow("adaptive", adaptive);
	imshow("adaptive_1", adaptive_1);
	waitKey(0);
	return 0;

}
