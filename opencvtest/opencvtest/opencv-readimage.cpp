#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <vector>

#define firstparam 10
#define secondparam 9
#define maxrad 20
#define nearrad 300
#define pathtofiles "C:\\Users\\sega0\\Desktop\\positive-20170718T173229Z-001\\positive\\"
#define filename "akn.002.078.left.avi"
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
	//load avis
	vector<String> listOfAvi;

	//input
		listOfAvi.push_back("akn.002.014.left.avi");
		listOfAvi.push_back("akn.002.040.left.avi");
		listOfAvi.push_back("akn.002.078.left.avi");
		listOfAvi.push_back("akn.002.084.left.avi");
		listOfAvi.push_back("akn.002.098.left.avi");
		listOfAvi.push_back("akn.002.135.left.avi");
		listOfAvi.push_back("akn.003.023.left.avi");
		listOfAvi.push_back("akn.003.027.left.avi");
		listOfAvi.push_back("akn.003.134.left.avi");
		listOfAvi.push_back("akn.019.037.left.avi");

		listOfAvi.push_back("akn.020.013.left.avi");
		listOfAvi.push_back("akn.024.084.left.avi");
		listOfAvi.push_back("akn.025.002.left.avi");
		listOfAvi.push_back("akn.025.015.left.avi");
		listOfAvi.push_back("akn.025.026.left.avi");
		listOfAvi.push_back("akn.026.021.left.avi");
		listOfAvi.push_back("akn.026.028.left.avi");
		listOfAvi.push_back("akn.028.024.left.avi");
		listOfAvi.push_back("akn.030.015.left.avi");
		listOfAvi.push_back("akn.030.039.left.avi");

		listOfAvi.push_back("akn.030.049.left.avi");
		listOfAvi.push_back("akn.031.016.left.avi");
		listOfAvi.push_back("akn.033.063.left.avi");
		listOfAvi.push_back("akn.033.073.left.avi");
		listOfAvi.push_back("akn.034.025.left.avi");
		listOfAvi.push_back("akn.035.017.left.avi");
		listOfAvi.push_back("akn.036.021.left.avi");
		listOfAvi.push_back("akn.036.050.left.avi");
		listOfAvi.push_back("akn.036.056.left.avi");
		listOfAvi.push_back("akn.060.018.left.avi");

		listOfAvi.push_back("akn.151.063.left.avi");
		listOfAvi.push_back("akn.151.079.left.avi");
		listOfAvi.push_back("akn.151.157.left.avi");
		listOfAvi.push_back("akn.152.015.left.avi");
		listOfAvi.push_back("akn.152.036.left.avi");
		listOfAvi.push_back("akn.154.010.left.avi");
		listOfAvi.push_back("akn.154.021.left.avi");
		listOfAvi.push_back("akn.154.059.left.avi");
		listOfAvi.push_back("akn.155.002.left.avi");
		listOfAvi.push_back("akn.155.008.left.avi");

		listOfAvi.push_back("akn.155.036.left.avi");
		listOfAvi.push_back("akn.155.052.left.avi");
		listOfAvi.push_back("akn.155.085.left.avi");
		listOfAvi.push_back("akn.156.108.left.avi");
		listOfAvi.push_back("akn.156.116.left.avi");
		listOfAvi.push_back("akn.157.023.left.avi");
		listOfAvi.push_back("akn.157.032.left.avi");
		listOfAvi.push_back("akn.158.044.left.avi");
		listOfAvi.push_back("akn.159.004.left.avi");
		listOfAvi.push_back("akn.160.012.left.avi");

	vector <Vec3f> circlesRad;
	vector <Vec3f> circlesGreen;

	for (int videoIndex = 0; videoIndex < listOfAvi.size(); ++videoIndex) {
		
		//load video
		VideoCapture capture(string(pathtofiles).append(listOfAvi[videoIndex]));
		if (!capture.isOpened()) std::cout << "Error when reading steam_avi";
		//pic init
		Mat frame;
		Mat onlyRad;
		Mat onlyGreen;
		//windows init
			//namedWindow(windowname, 1);
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
			inRange(frame, Scalar(167, 172, 0), Scalar(237, 242, 103), onlyGreen);

			//find cercle
			HoughCircles(onlyRad, circlesRad, CV_HOUGH_GRADIENT, 1, onlyRad.rows / 16, firstparam, secondparam, 0, maxrad);
			HoughCircles(onlyGreen, circlesGreen, CV_HOUGH_GRADIENT, 1, onlyGreen.rows / 16, firstparam, secondparam, 0, maxrad);

			//drow circles
			for (size_t i = 0; i < circlesRad.size(); i++) {
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
				cout << "frame: " << l << "\r";
				/*cout << "rad:" << circlesRad.size() << endl;
				for (int i = 0; i < circlesRad.size(); ++i) {
					cout << "\t(" << circlesRad[i][0] << ";" << circlesRad[i][1] << ")\n";
				}
				cout << "green:" << circlesGreen.size() << endl;
				for (int i = 0; i < circlesGreen.size(); ++i) {
					cout << "\t(" << circlesGreen[i][0] << ";" << circlesGreen[i][1] << ")\n";
				}*/

			//show
				//imshow(windowname, frame);
				//waitKey(1);

			//verdict
			bool continueFlag = false;
			for (int i = 0; i < circlesGreen.size(); ++i) {
				for (int j = 0; j < history_vec.size(); j++) {
					for (int k = 0; k < history_vec[j].size(); ++k) {
						if (isNear(circlesGreen[i], history_vec[j][k])) {
							cout << listOfAvi[videoIndex] << "\t" << l << endl;
							continueFlag= true;
								//waitKey(0);
						}
						if (continueFlag) break;
					}
					if (continueFlag) break;
				}
				if (continueFlag) break;
			}
			if (continueFlag) break;
		}
	
	}
	system("pause");
	return 0;
}