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

int findCircle(Mat image, vector <Vec3f> circles, int param1, int param2) {
	HoughCircles(image, circles, CV_HOUGH_GRADIENT, 1, image.rows / 16, param1, param2, 0, 0);
	return circles.size();
}

int main() {
	namedWindow("w", 1);
	Mat image = imread("C:\\Users\\sega0\\Desktop\\git_tmp\\opencv\\opencvtest\\opencvtest\\11.jpg", 1);
	Mat image_gray;
	cvtColor(image, image_gray, CV_BGR2GRAY);
	//GaussianBlur(image, image, Size(9, 9), 2, 2);
	//imshow("w", image);
	//waitKey(0);
	vector <Vec3f> circlesRad;
	vector <Vec3f> circlesGreen;

	//int count = 0;
	//for (int i = 0; i < 200; i++)
	//{
	//	for (int j = 0; j < 200; j++) {
	//		cout << i << ":" << j << "\r";
	//		try
	//		{
	//			count = findCircle(image_gray, circles, i, j);
	//		}
	//		catch(exception e)
	//		{
	//			//cout << "error " << i << "/" << j << endl;
	//		}
	//		if (count > 0) {
	//			cout << endl<< "success!" << i << ":" << j<<":"<<count << endl;
	//		}
	//		count = 0;
	//	}
	//}


	/*HoughCircles(image_gray, circles, CV_HOUGH_GRADIENT, 1, image.rows / 16, 34, 10, 0, 0);
	for (size_t i = 0; i < circles.size(); i++)
	
	{
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius = cvRound(circles[i][2]);
	circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
	circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}*/
	
	//imshow("w", image);
	//waitKey(0);
	//////////
	//load file
	VideoCapture capture("C:\\Users\\sega0\\Desktop\\git_tmp\\opencv\\opencvtest\\opencvtest\\test2.avi");
	if (!capture.isOpened()) std::cout << "Error when reading steam_avi";
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
	namedWindow("rad", 1);
	//namedWindow("green", 1);
	//processing
	
	for (int l = 0;; ++l) {
		capture >> frame;

		if (frame.empty()) std::cout << "empty\n";
		if (frame.empty()) break;

		//detect colors
		inRange(frame, Scalar(44, 19, 174), Scalar(114, 89, 244), onlyRad);
		inRange(frame, Scalar(167, 172, 33), Scalar(237, 242, 103), onlyGreen);

		//int count = 0;
		//for (int i = 0; i < 200; i++)
		//{
		//	for (int j = 6; j < 10; j++) {
		//		cout << i << ":" << j << "\r";
		//		try
		//		{
		//			count = findCircle(onlyRad, circles, i, j);
		//		}
		//		catch(exception e)
		//		{
		//			//cout << "error " << i << "/" << j << endl;
		//		}
		//		if ((count > 1)&&(count<4)) {
		//			cout << endl<< "success!" << i << ":" << j<<":"<<count << endl;
		//		}
		//		count = 0;
		//	}
		//}



		//find cercle
		//Mat image_gray;
		//cvtColor(onlyRad, image_gray, CV_BGR2GRAY);
		HoughCircles(onlyRad, circlesRad, CV_HOUGH_GRADIENT, 1, onlyRad.rows / 16, 10, 9, 0, 20);
		HoughCircles(onlyGreen, circlesGreen, CV_HOUGH_GRADIENT, 1, onlyGreen.rows / 16, 10, 9, 0, 20);
		//cout << circles.size() << endl;
		for (size_t i = 0; i < circlesRad.size(); i++){
		Point center(cvRound(circlesRad[i][0]), cvRound(circlesRad[i][1]));
		int radius = cvRound(circlesRad[i][2]);
		circle(frame, center, 3, Scalar(0, 0, 255), -1, 8, 0);
		circle(frame, center, radius, Scalar(255, 0, 0), 3, 8, 0);
		}
		for (size_t i = 0; i < circlesGreen.size(); i++) {
			Point center(cvRound(circlesGreen[i][0]), cvRound(circlesGreen[i][1]));
			int radius = cvRound(circlesGreen[i][2]);
			circle(frame, center, 3, Scalar(0, 0, 255), -1, 8, 0);
			circle(frame, center, radius, Scalar(0, 255, 0), 3, 8, 0);
		}
		imshow("rad", frame);

		waitKey(20);
	}

	system("pause");
	return 0;
}