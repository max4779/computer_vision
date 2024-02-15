#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	Mat frame;
	VideoCapture cap;
	double current_frames;
	

	if (cap.open("background.mp4") == 0) {
		cout << "No file" << endl;
		waitKey(0);
	}

	int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
	double fps = cap.get(CAP_PROP_FPS);
	int delay = total_frames / fps;

	double limit_time;
	bool stop=true;

	while (stop) {	
		cap >> frame;
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}
		current_frames = cap.get(CAP_PROP_POS_FRAMES);
		imshow("video", frame);
		cout << "frame: " << current_frames << " / " << total_frames << endl;
		waitKey(delay);

		limit_time = cap.get(CAP_PROP_POS_MSEC);
		if (limit_time > 3000) {
			stop = false;
		}
	}

}