#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams
{
    Mat img;
    VideoCapture background, input;
    vector<Point2f> in, out;
    int maxX, maxY, minX, minY, firstX,firstY;
    bool first=true;
    
};
static void onMouse(int event, int x, int y, int, void* param)
{
    MouseParams* mp = (MouseParams*)param;
    Mat img = mp->img;

    if (event == EVENT_LBUTTONDOWN)
    {
        mp->out.push_back(Point2f(x, y));
        if (x > mp->maxX) {
            mp->maxX = x;
        }
        if (y > mp->maxY) {
            mp->maxY = y;
        }
        if (x < mp->minX) {
            mp->minX = x;
        }
        if (y < mp->minY) {
            mp->minY = y;
        }
        if (mp->first) {
            mp->firstX = x;
            mp->firstY = y;
            mp->first = false;
        }
    }
    if (event == EVENT_RBUTTONDOWN)
    {
        mp->out.clear();
        imshow("input", img);
    }
}


int main()
{
    VideoCapture cap1("contest.mp4");
    VideoCapture cap2("Timesquare.mp4");
    Mat frame1, frame2, result, ori, warp;
    if (cap1.open("contest.mp4") == 0)
    {
        cout << "There is no contest file" << endl;
        return -1;
    }
    if (cap2.open("Timesquare.mp4") == 0)
    {
        cout << "There is no Timesqure file" << endl;
        return -1;
    }

    cap1 >> frame1;
    MouseParams mp;

    mp.in.push_back(Point2f(0, 0));
    mp.in.push_back(Point2f(frame1.cols - 1, 0));
    mp.in.push_back(Point2f(frame1.cols - 1, frame1.rows - 1));
    mp.in.push_back(Point2f(0, frame1.rows - 1));

    mp.maxX = 0;
    mp.maxY = 0;
    mp.minX = frame1.cols - 1;
    mp.minY = frame1.rows - 1;


    while (true) {
        cap1 >> frame1;
        cap2 >> frame2;
;
        if (frame1.empty() || frame2.empty()) {
            cout << "end of video" << endl;
            break;
        }

        mp.img = frame1;
        result = frame2.clone();
        if (mp.out.size() == 4)
        {
            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);

            warpPerspective(mp.img, warp, homo_mat, Size(mp.maxX,mp.maxY));
            warp.copyTo(result((Rect(0, 0, warp.cols, warp.rows))));


            imshow("background", result);
        }

        else 
        { 
            for (size_t i = 0; i < mp.out.size(); i++)
            {
                circle(result, mp.out[i], 3, Scalar(0, 0, 255), 5);
            }
            imshow("background", result);
        }
        imshow("input", frame1);   
        setMouseCallback("background", onMouse, (void*)&mp);
        waitKey(33);
    }
    return 0;
}
