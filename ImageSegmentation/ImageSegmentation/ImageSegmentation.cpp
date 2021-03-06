#include "stdafx.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	Mat image;

	if (argc != 2) {
		cout << "Two arguments are needed" << endl;
		return -1;
	}

	image = imread(argv[1], IMREAD_COLOR);

	if (!image.data) {
		cout << "Image load faild" << endl;
		return -1;
	}

	int h = image.rows;
	int w = image.cols;

	cout << "Height: " << h << endl;
	cout << "Width: " << w << endl;

	Mat grayImage;
	Mat strechedImage(h, w, CV_8UC1, Scalar(255));

	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	for (int x = 0; x < h; x++) {
		for (int y = 0; y < w; y++) {
			int a = grayImage.at<uchar>(x, y);

			if (a < 80)
				strechedImage.at<uchar>(x, y) = cvRound(double(a) * (10.0 / 80.0));
			else if (a < 230)
				strechedImage.at<uchar>(x, y) = cvRound(double(a - 80) * (235.0 / 150.0)) + 10;
			else
				strechedImage.at<uchar>(x, y) = cvRound(double(a - 230) * (10.0 / 25.0)) + 245;
		}
	}

	int histogram[256] = { 0 };

	for (int x = 0; x < h; x++) {
		for (int y = 0; y < w; y++) {
			int a = strechedImage.at<uchar>(x, y);
			histogram[a]++;
		}
	}

	int max = 0;
	for (int x = 0; x < 256; x++) {
		if (max < histogram[x]) {
			max = histogram[x];
		}
	}

	Mat histImage(300, 260, CV_8UC1, Scalar(255));

	int hist[256];
	double maxd = max;
	for (int x = 0; x < 256; x++) {
		cout << x << " : " << histogram[x] << endl;
		hist[x] = cvRound(double(histogram[x] / maxd) * 300);
		Point pt1 = Point(x, 300 - hist[x]);
		Point pt2 = Point(x, 300);

		line(histImage, pt1, pt2, Scalar(0), 1, 8, 0);
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", strechedImage);

	namedWindow("Histogram Window", WINDOW_AUTOSIZE);
	imshow("Histogram Window", histImage);

	waitKey(0);
	return 0;
}