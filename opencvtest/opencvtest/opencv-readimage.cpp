#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <vector>

#define firstparam 10
#define secondparam 9
#define maxrad 20
#define nearrad 300
#define filename "test2.avi"
#define windowname "original"

using namespace cv;
using namespace std;

class Last10rad {
private:
	vector< vector<Vec3f> > vec;
public:
	vector< vector<Vec3f> > getVec();
	void push(vector<Vec3f> item);
};

vector<vector<Vec3f> > Last10rad::getVec() {
	return vec;
}

void Last10rad::push(vector<Vec3f> item) {
	vec.push_back(item);
	if (vec.size() > 20) {
		vec.erase(vec.begin());
	}
	return;
}

bool isNear(Vec3f first, Vec3f second) {
	double dist = sqrt((first[0] - second[0])*(first[0] - second[0]) + (first[1] - second[1])*(first[1] - second[1]));
	if (dist < nearrad) return true;
	return false;
}

int main() {
	vector <Vec3f> circlesRad;
	vector <Vec3f> circlesGreen;

	//load video
	VideoCapture capture(string("C:\\Users\\sega0\\Desktop\\git_tmp\\opencv\\opencvtest\\opencvtest\\").append(filename));
	if (!capture.isOpened()) std::cout << "Error when reading steam_avi";
	//pic init
	Mat frame;
	Mat onlyRad;
	Mat onlyGreen;
	//windows init
	namedWindow(windowname, 1);
	//processing
	
	Last10rad history = *(new Last10rad());

	for (int l = 0;; ++l) {
		//get next frame
		capture >> frame;

		//check
		if (frame.empty()) std::cout << "empty\n";
		if (frame.empty()) break;

		//detect colors
		inRange(frame, Scalar(44, 19, 174), Scalar(114, 89, 244), onlyRad);
		inRange(frame, Scalar(167, 172, 33), Scalar(237, 242, 103), onlyGreen);

		//find cercle
		HoughCircles(onlyRad, circlesRad, CV_HOUGH_GRADIENT, 1, onlyRad.rows / 16, firstparam, secondparam, 0, maxrad);
		HoughCircles(onlyGreen, circlesGreen, CV_HOUGH_GRADIENT, 1, onlyGreen.rows / 16, firstparam, secondparam, 0, maxrad);

		//drow circles
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

		history.push(circlesRad);
		vector< vector<Vec3f> > history_vec = history.getVec();

		//textinfo
		cout << "rad:" << circlesRad.size() << endl;
		for (int i = 0; i < circlesRad.size(); ++i) {
			cout << "\t(" << circlesRad[i][0] << ";" << circlesRad[i][1] << ")\n";
		}
		cout << "green:" << circlesGreen.size() << endl;
		for (int i = 0; i < circlesGreen.size(); ++i) {
			cout << "\t(" << circlesGreen[i][0] << ";" << circlesGreen[i][1] << ")\n";
		}
		//verdict
		for (int i = 0; i < circlesGreen.size(); ++i) {
			for (int j = 0; j < history_vec.size(); j++) {
				for (int k = 0; k < history_vec[j].size(); ++k) {
					if (isNear(circlesGreen[i], history_vec[j][k])) {
						cout << "**** " << l -5 << " ****";
						waitKey(0);
					}
				}
			}
		}

		imshow(windowname, frame);
		waitKey(10);
	}

	system("pause");
	return 0;
}