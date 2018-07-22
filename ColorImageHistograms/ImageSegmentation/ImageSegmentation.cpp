// ImageSegmentation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" //main opencv header
#include "highgui.h" //GUI header
#include "core/mat.hpp"
#include "imgcodecs.hpp"
#include "highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

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

	//Retrieve Pixel Color
	int b = image.at<Vec3b>(0, 0)[0];

	cout << "Height: " << h << endl;
	cout << "Width: " << w << endl;


	Mat bImage(h, w, CV_8UC3, Scalar(0));
	Mat gImage(h, w, CV_8UC3, Scalar(0));
	Mat rImage(h, w, CV_8UC3, Scalar(0));


	int histogram[3][256] = { 0 };

	for (int x = 0; x < h; x++) {
		for (int y = 0; y < w; y++) {
			int b = image.at<Vec3b>(x, y)[0];
			int g = image.at<Vec3b>(x, y)[1];
			int r = image.at<Vec3b>(x, y)[2];

			bImage.at<Vec3b>(x, y)[0] = b;
			gImage.at<Vec3b>(x, y)[1] = g;
			rImage.at<Vec3b>(x, y)[2] = r;

			histogram[0][b]++;
			histogram[1][g]++;
			histogram[2][r]++;
		}
	}

	int bMax =0, gMax = 0, rMax = 0;
	for (int x = 0; x < 256; x++) {
		if (bMax < histogram[0][x]) {
			bMax = histogram[0][x];
		}
		if (gMax < histogram[1][x]) {
			gMax = histogram[1][x];
		}
		if (rMax < histogram[2][x]) {
			rMax = histogram[2][x];
		}
	}

	Mat bHistImage(300, 260, CV_8UC1, Scalar(255));
	Mat gHistImage(300, 260, CV_8UC1, Scalar(255));
	Mat rHistImage(300, 260, CV_8UC1, Scalar(255));

	int hist[3][256];
	double bmaxd = bMax, gmaxd = gMax, rmaxd = rMax;
	for (int x = 0; x < 256; x++) {
		// Blue hisogram lines
		hist[0][x] = cvRound(double(histogram[0][x] / bmaxd) * 300);
		Point bpt1 = Point(x, 300 - hist[0][x]);
		Point bpt2 = Point(x, 300);

		line(bHistImage, bpt1, bpt2, Scalar(0), 1, 8, 0);

		// Green hisogram lines
		hist[1][x] = cvRound(double(histogram[1][x] / bmaxd) * 300);
		Point gpt1 = Point(x, 300 - hist[1][x]);
		Point gpt2 = Point(x, 300);

		line(gHistImage, gpt1, gpt2, Scalar(0), 1, 8, 0);

		// Blue hisogram lines
		hist[2][x] = cvRound(double(histogram[2][x] / bmaxd) * 300);
		Point rpt1 = Point(x, 300 - hist[2][x]);
		Point rpt2 = Point(x, 300);

		line(rHistImage, rpt1, rpt2, Scalar(0), 1, 8, 0);
	}


	namedWindow("Blue Histogram", WINDOW_AUTOSIZE);
	imshow("Blue Histogram", bHistImage);

	namedWindow("Green Histogram", WINDOW_AUTOSIZE);
	imshow("Green Histogram", gHistImage);

	namedWindow("Red Histogram", WINDOW_AUTOSIZE);
	imshow("Red Histogram", rHistImage);

	namedWindow("Blue Image", WINDOW_AUTOSIZE);
	imshow("Blue Image", bImage);

	namedWindow("Green Image", WINDOW_AUTOSIZE);
	imshow("Green Image", gImage);

	namedWindow("Red Histogram", WINDOW_AUTOSIZE);
	imshow("Red Image", rImage);

	//Mat grayImage;
	//Mat strechedImage(w, h, CV_8UC1, Scalar(255));


	//cvtColor(image, grayImage, COLOR_BGR2GRAY);

	//for (int x = 0; x < h; x++) {
	//	for (int y = 0; y < w; y++) {
	//		int a = grayImage.at<uchar>(x, y);

	//		if (a <= 114)
	//			strechedImage.at<uchar>(x, y) = cvRound(double(a) * (20.0 / 114.0));
	//		else if (a <= 242)
	//			strechedImage.at<uchar>(x, y) = cvRound(double(a - 114) * (230.0 / 128.0)) + 20;
	//		else
	//			strechedImage.at<uchar>(x, y) = cvRound(double(a - 242) * (5.0 / 13.0)) + 250;
	//	}
	//}

	//int histogram[256] = { 0 };

	//for (int x = 0; x < h; x++) {
	//	for (int y = 0; y < w; y++) {
	//		int a = strechedImage.at<uchar>(x, y);
	//		histogram[a]++;
	//	}
	//}

	//int max = 0;
	//for (int x = 0; x < 256; x++) {
	//	if (max < histogram[x]) {
	//		max = histogram[x];
	//	}
	//}

	//Mat histImage(300, 260, CV_8UC1, Scalar(255));

	//int hist[256];
	//double maxd = max;
	//for (int x = 0; x < 256; x++) {
	//	cout << x << " : " << histogram[x] << endl;
	//	hist[x] = cvRound(double(histogram[x] / maxd) * 300);
	//	Point pt1 = Point(x, 300 - hist[x]);
	//	Point pt2 = Point(x, 300);

	//	line(histImage, pt1, pt2, Scalar(0), 1, 8, 0);
	//}


	//namedWindow("Display window", WINDOW_AUTOSIZE);
	//imshow("Display window", strechedImage);

	//namedWindow("Histogram Window", WINDOW_AUTOSIZE);
	//imshow("Histogram window", histImage);

	waitKey(0);
	return 0;
}