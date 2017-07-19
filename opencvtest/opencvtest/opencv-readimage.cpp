#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <vector>
#include <Windows.h>

#define firstparam 10
#define secondparam 9
#define maxrad 20
#define nearrad 300
#define pathtofiles "C:\\Users\\sega0\\Desktop\\positive-20170718T173229Z-001\\positive\\"
#define filename "akn.002.078.left.avi"
#define windowname "original"

bool match(char const *needle, char const *haystack) {
	for (; *needle != '\0'; ++needle) {
		switch (*needle) {
		case '?':
			if (*haystack == '\0')
				return false;
			++haystack;
			break;
		case '*': {
			if (needle[1] == '\0')
				return true;
			size_t max = strlen(haystack);
			for (size_t i = 0; i < max; i++)
				if (match(needle + 1, haystack + i))
					return true;
			return false;
		}
		default:
			if (*haystack != *needle)
				return false;
			++haystack;
		}
	}
	return *haystack == '\0';
}

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

	//getInput
	string path;
	//cin >> path;
	path = "C:\\Users\\sega0\\Desktop\\positive-20170718T173229Z-001\\positive\\akn.003*.avi";
	int ind = path.find_last_of("\\");
	string mask = path.substr(ind + 1);
	path = path.substr(0, ind + 1);
	//cout << "path: "<< path <<"\n";
	//cout << "mask: "<< mask <<"\n";
	path = path.append("*");
	WIN32_FIND_DATA  FindFileData;
	// wstring st = wstring(path.begin(), path.end());
	///  LPCWSTR lpath = st.c_str();
	// cout « lpath;
	HANDLE const hf = FindFirstFile(path.c_str(), &FindFileData);
	const char *msk = mask.c_str();
	if (INVALID_HANDLE_VALUE != hf)
	{
		do
		{
			string name = &FindFileData.cFileName[0];
			if (name != "." && name != "..")
			{
				const char *nme = name.c_str();
				if (match(msk, nme)) {
					listOfAvi.push_back(nme);
				}
				//cout << nme <<  " " << boolalpha << match(msk, nme) << "\n";
			}

		} while (NULL != FindNextFile(hf, &FindFileData));

		FindClose(hf);
	}

	vector <Vec3f> circlesRad;
	vector <Vec3f> circlesGreen;

	for (int videoIndex = 0; videoIndex < listOfAvi.size(); ++videoIndex) {
		
		VideoCapture capture;

		//load video
		try {
			capture = *(new VideoCapture(string(pathtofiles).append(listOfAvi[videoIndex])));
			if (!capture.isOpened()) std::cout << "Error when reading steam_avi";
		}
		catch (exception e) {
			cout << -2;
			return 0;
		}
		
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
			if (frame.empty()) {
				cout << listOfAvi[videoIndex] << "\t" << -1<<endl;
				break;
			}

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