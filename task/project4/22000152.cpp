#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    string imageNames[4] = { "pano1.jpg","pano2.jpg" ,"pano3.jpg" ,"pano4.jpg" };
    Mat images[4], descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(2000);
    vector<KeyPoint> keypoints1, keypoints2;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int k = 2;
    float nndr;

    images[0] = imread(imageNames[0]);
    Mat IMG = images[0].clone();
    resize(IMG, IMG, Size(640, 480));

    for (int j = 1; j < 4; j++) {
        vector<vector<DMatch>> matches;
        vector<DMatch> goodMatches;

        images[j] = imread(imageNames[j]);
        resize(images[j], images[j], Size(640, 480));

        orbF->detectAndCompute(IMG, noArray(), keypoints1, descriptors1);
        orbF->detectAndCompute(images[j], noArray(), keypoints2, descriptors2);
        matcher.knnMatch(descriptors1, descriptors2, matches, k);
        nndr = 0.4f;

        for (int i = 0; i < matches.size(); i++) {
            if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
                goodMatches.push_back(matches[i][0]);
            }
        }

        vector<Point2f> pt1, pt2;
        for (int o = 0; o < goodMatches.size(); o++) {
            int first = goodMatches[o].queryIdx;
            int second = goodMatches[o].trainIdx;
            pt1.push_back(keypoints1[first].pt);
            pt2.push_back(keypoints2[second].pt);
        }

        Mat homo = findHomography(pt1, pt2, RANSAC);
        Mat result;
        warpPerspective(images[j], result, homo, Size(IMG.cols, IMG.rows));

        Mat combinedImage = Mat::zeros(IMG.rows * 1.5, IMG.cols * 2, IMG.type());

        Mat left(IMG, Rect(0, 0, IMG.cols, IMG.rows));
        left.copyTo(combinedImage(Rect(0, 0, IMG.cols, IMG.rows)));

        if (j == 1) {
            result(Rect(0, 0, IMG.cols, IMG.rows)).copyTo(combinedImage(Rect(IMG.cols / 2 ^ (j), 0, IMG.cols, IMG.rows)));
        }
        else if (j == 2) {
            result(Rect(0, 0, IMG.cols, IMG.rows)).copyTo(combinedImage(Rect(IMG.cols / 2.5 , 0, IMG.cols, IMG.rows)));
        }
        else if (j == 3) {
            result(Rect(0, 0, IMG.cols, IMG.rows)).copyTo(combinedImage(Rect(IMG.cols / 3.5, 0, IMG.cols, IMG.rows)));
        }
        IMG = combinedImage.clone();
    }

    int colorx = 0, colory = 0;
    for (int y = 0; y < IMG.rows; y++) {
        for (int x = 0; x < IMG.cols; x++) {
            if (IMG.at<Vec3b>(y, x)[0] == 0 && IMG.at<Vec3b>(y, x)[1] == 0 && IMG.at<Vec3b>(y, x)[2] == 0) {
                continue;
            }
            if (colorx < x) {
                colorx = x;
            }
            if (colory < y) {
                colory = y;
            }
        }
    }

    Mat result;
    result = IMG(Range(0, colory), Range(0, colorx));
    imshow("panorama_img", result);

    waitKey(0);
    return 0;
}
