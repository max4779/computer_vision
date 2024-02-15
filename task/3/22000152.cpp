#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	Mat moon = imread("Moon.jpeg", 0);
	Mat moon_change = moon.clone();
	Rect rect((moon.cols) / 2, 0, moon.cols / 2, moon.rows);
	Mat moon_filtered=moon_change(rect);
	Mat laplacian, abs_laplacian;
	
	GaussianBlur(moon_filtered, moon_filtered, Size(3, 3), 0, 0, BORDER_DEFAULT);

	Laplacian(moon_filtered, laplacian, CV_16S, 1, 1, 0);
	convertScaleAbs(laplacian, abs_laplacian);
	moon_filtered = abs_laplacian + moon_filtered;

	imshow("moon", moon);
	imshow("moon_filtered", moon_change);



	Mat saltnpepper = imread("saltnpepper.png", 0);
	Mat saltnpepper_filtered = saltnpepper.clone();
	Rect rect_2(0, 0, (saltnpepper.cols) / 2, saltnpepper.rows);

	medianBlur(saltnpepper_filtered(rect_2), saltnpepper_filtered(rect_2), 9);

	imshow("saltnpepper", saltnpepper);
	imshow("saltnpepper_filtered", saltnpepper_filtered);


	waitKey(0);
}