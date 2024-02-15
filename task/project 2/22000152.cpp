#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main()
{
    VideoCapture cap("Project2_video.mp4");

    if (cap.open("Project2_video.mp4") == 0)
    {
        cout << "There is no file" << endl;
        return -1;
    }

    Mat frame, result, frame2, frame_FCM, MCM, FCM, FCM_area,thresh, ismoving;

    vector<Vec4i> lines1, lines2, lines3;

    Rect left_roi(200, 330, 70, 70);
    Rect right_roi(430, 330, 70, 70);
    Rect center_roi(300, 330, 100, 70);
    Rect FCM_roi(270, 250, 150, 140);
    bool start = false;
    bool left_on = false, right_on = false, center_on = false, waiting_time = false, waiting_time2=false;
    Mat left, right, center;

    int fps = cap.get(CAP_PROP_FPS);
    int time = 0, time2 = 0, post_time = 0, post_time2 = 0;
    while (1) {

        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }
        time = cap.get(CAP_PROP_POS_MSEC);
        time2 = cap.get(CAP_PROP_POS_MSEC);
        result = frame.clone();
        Rect MoveCheck(0, frame.rows/2, frame.cols, frame.rows/2);


        MCM = frame(MoveCheck);
        cap.grab();
        cap.retrieve(frame2);
        resize(frame2, frame2, MCM.size());
        absdiff(MCM, frame2, ismoving);
        threshold(ismoving, ismoving, 150, 170, THRESH_BINARY);



        int moving_count=0;
        for (int i = 0; i < ismoving.rows; i++) {
            for (int j = 0; j < ismoving.cols; j++) {
                if (ismoving.at<uchar>(i, j) > 0) {
                    moving_count++;
                }
            }
        }
        if (moving_count < 7000) {
            FCM_area = frame(FCM_roi);
            cap.retrieve(frame_FCM);
            resize(frame_FCM, frame_FCM, FCM_area.size());
            absdiff(FCM_area, frame_FCM, FCM);
            threshold(FCM, FCM, 200, 230, THRESH_BINARY);


            int FCM_count = 0;
            for (int i = 0; i < FCM.rows; i++) {
                for (int j = 0; j < FCM.cols; j++) {
                    if (FCM.at<uchar>(i, j) > 0) {
                        FCM_count++;
                    }
                }
            }
            if (FCM_count > 7 && FCM_count<9) {
                waiting_time2 = true;
                post_time2 = time + 1000;
            }
        }

        else {
            left = frame(left_roi);
            cvtColor(left, left, COLOR_BGR2GRAY);
            Canny(left, left, 50, 80, 3);

            right = frame(right_roi);
            cvtColor(right, right, COLOR_BGR2GRAY);
            Canny(right, right, 50, 80, 3);

            center = frame(center_roi);
            cvtColor(center, center, COLOR_BGR2GRAY);
            Canny(center, center, 50, 80, 3);

            HoughLinesP(left, lines1, 1, CV_PI / 180, 30, 50, 20);
            HoughLinesP(right, lines2, 1, CV_PI / 180, 30, 50, 20);
            HoughLinesP(center, lines3, 1, CV_PI / 180, 30, 50, 20);


            for (size_t i = 0; i < lines1.size(); i++)
            {
                Vec4i l = lines1[i];
                double x = l[2] - l[0];
                double y = l[3] - l[1];

                if (x / y < 0 && abs(x / y) < 3) {
                    left_on = true;
                }
            }

            for (size_t i = 0; i < lines2.size(); i++)
            {
                Vec4i l = lines2[i];
                double x = l[2] - l[0];
                double y = l[3] - l[1];

                if (x / y > 0 && abs(x / y) < 3) {             
                    right_on = true;
                }
            }

            for (size_t i = 0; i < lines3.size(); i++)
            {
                Vec4i l = lines3[i];
                double x = l[2] - l[0];
                double y = l[3] - l[1];

                if (abs(x / y) < 1) {
                    center_on = true;
                }
            }
        }


        if (center_on && !(left_on) && !(right_on)) {
            waiting_time = true;
            post_time = time + 1000;

        }

        

        if (waiting_time) {
            if (post_time > time) {
                putText(result, format("Lane departure!"), Point(180, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 5);
            }
            else {
                waiting_time = false;
            }
            
        }

        if (waiting_time2) {
            if (post_time2 > time2) {
                putText(result, format("Start Moving"), Point(180, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 5);
            }
            else {
                waiting_time2 = false;
            }

        }

        imshow("Project2", result);


        left_on = false;
        right_on = false;
        center_on = false;


        waitKey(1000 / fps);
    }






    cap.release();

    return 0;
}
