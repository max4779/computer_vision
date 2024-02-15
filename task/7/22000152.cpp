#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;




int main()
{
    VideoCapture cap("Faces.mp4");
    int fps = cap.get(CAP_PROP_FPS);

    if (cap.open("Faces.mp4") == 0)
    {
        cout << "There is no file" << endl;
        return -1;
    }
    Rect m_rc;
    CascadeClassifier face_classifier;
    face_classifier.load("haarcascade_frontalface_alt.xml");
    Mat frame, framet, grayframe, track;
    vector<Rect> faces;
    int input = 0;
    int i;
    int min = 0, max = 0;

    bool fkey = false, mkey = false, nkey = false, tracking=false;



    while (true) {
        cap >> frame;
        cap >> framet;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }

        input = waitKey(10);

        cvtColor(frame, track, COLOR_BGR2HSV);
        cvtColor(frame, grayframe, COLOR_BGR2GRAY);
        

        if (waitKey(33) == 27) {
            break;
        }
        else if (input == 'f') {
            fkey = true;
            mkey = false;
            nkey = false;
            tracking = false;
        }
        else if (input == 'm') {
            fkey = false;
            mkey = true;
            nkey = false;
            tracking = false;
        }

        else if (input == 'n') {
            fkey = false;
            mkey = false;
            nkey = true;
            tracking = false;
        }
        else if (input == 'r') {
            fkey = false;
            mkey = false;
            nkey = false;
            tracking = false;
        }
        else if (input == 't') {
            if (tracking == true) {
                tracking = false;
                destroyWindow("Tracking");
            }
            else {
                tracking = true;
            }
        }
        
        if (fkey) {
            min = 20;
            max = 55;
        }
        if (mkey) {
            min = 55;
            max = 65;
        }
        if (nkey) {
            min = 71;
            max = 90;
        }
        
        

        if ((fkey || mkey || nkey) && tracking) {
            Mat mask(frame.size(), CV_8U, Scalar(255));

            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(min, max), Size(max, max));
            for (i = 0; i < faces.size(); i++) {
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                Point tr(faces[i].x, faces[i].y);
                rectangle(mask, lb, tr, Scalar(0), -1);
            }

            framet.setTo(Scalar(128, 0, 0));

            frame.copyTo(framet, ~mask);

            imshow("Tracking", framet);
        }

        else if (fkey || mkey || nkey) {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(min, max), Size(max, max));
            for (i = 0; i < faces.size(); i++) {
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                Point tr(faces[i].x, faces[i].y);
                rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
            }
            
            
        }
        else if(!(fkey || mkey || nkey) && tracking){
            putText(frame, format("Detect before Tracking"), Point(180, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 5);
        }
        imshow("Face Detection", frame);
        
        
    }


}
