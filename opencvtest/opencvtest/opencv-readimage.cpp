#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <vector>

#define EPS 50

using namespace cv;
using namespace std;

bool isequal(int first, int second, int eps) {
	if (abs(first - second) <= eps) return true;
	return false;
}

int main() {
	//load file
	VideoCapture capture("C:\\Users\\sega0\\Desktop\\git_tmp\\opencv\\opencvtest\\opencvtest\\test.avi");
	if (!capture.isOpened()) std::cout<<"Error when reading steam_avi";
	//pic init
	Mat frame;
	Mat onlyRad;
	Mat onlyGreen;
	//vectors init
	vector<int> rad;
	vector<int> green;
	vector<Mat> radMat;
	vector<Mat> greenMat;
	//windows init;
	//namedWindow("original", 1);
	//namedWindow("rad", 1);
	//namedWindow("green", 1);
	//processing
	for (int i=0;;++i) {
		capture >> frame;
		
		if (frame.empty()) std::cout << "empty\n";
		if (frame.empty()) break;
		
		//detect colors
		inRange(frame, Scalar(44, 19, 174), Scalar(114,89, 244), onlyRad);
		inRange(frame, Scalar(167, 172, 33), Scalar(237, 242, 103), onlyGreen);
		
		//calc
		int radPixels = countNonZero(onlyRad);
		int greenPixels = countNonZero(onlyGreen);

		//put in vector
		rad.push_back(radPixels);
		radMat.push_back(onlyRad);
		green.push_back(greenPixels);
		greenMat.push_back(onlyGreen);
		
		//visualization
		//imshow("original", frame);
		//imshow("rad", onlyRad);
		//imshow("green", onlyGreen);

		//wait
		//waitKey(20);
		cout << i<< "\r";
	}

	system("pause");
}