#include "stdafx.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat draw_histogram(Mat img);
Mat contrast_stretch(Mat img, double a, double b, double c, double d);

//take two cmds from cmd promt
int main(int argc, char **argv) {

	//no of cmds
	if (argc != 2) {
		cout << " Usage: display_image ImageToLoadAndDisplay " << endl;
		return -1;
	}

	Mat gr;
	Mat gr1;

	gr = imread(argv[1], IMREAD_GRAYSCALE);
	gr1 = imread(argv[1], IMREAD_GRAYSCALE);

	////for color image
	//gr = imread(argv[1], IMREAD_COLOR);
	//gr1 = imread(argv[1], IMREAD_COLOR);

	/*check for invalid input*/
	if (!gr.data) {
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	int h = gr.rows;
	int w = gr.cols;

	cout << "Height : " << h << endl;
	cout << "Width : " << w << endl;

	Mat histImage1 = draw_histogram(gr);
	Mat enhancedImg = contrast_stretch(gr1, 115.0, 220.0, 10.0, 245.0);
	Mat histImage2 = draw_histogram(enhancedImg);

	//save the enhanced image
	cv::imwrite("enhanced_img1.bmp", enhancedImg);

	namedWindow("Display Window 1", WINDOW_AUTOSIZE);
	imshow("Display Window 1", gr);

	namedWindow("Histogram 1", WINDOW_AUTOSIZE);
	imshow("Histogram 1", histImage1);

	namedWindow("Display Window 2", WINDOW_AUTOSIZE);
	imshow("Display Window 2", enhancedImg);

	namedWindow("Histogram 2", WINDOW_AUTOSIZE);
	imshow("Histogram 2", histImage2);

	waitKey(0); // Wait for a keystroke in the window
	gr.release();
	histImage1.release();
	enhancedImg.release();
	histImage2.release();

	return 0;
}

Mat draw_histogram(Mat img) {
	int h = img.rows;
	int w = img.cols;

	/*Obtain no of pixels for each grey value*/
	int levels[256] = { 0 };
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			int val = img.at<uchar>(i, j);
			levels[val] += 1;
		}
	}
	int max = 0;
	for (int i = 0; i < 256; i++)
	{
		cout << "For colour level " << i << ": " << levels[i] << endl;
		if (max < levels[i])
		{
			max = levels[i];
		}
	}
	cout << "Maximum value: " << max;

	int hist_w = 260; int hist_h = 301;
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255)); //Draw histogram window of white color

	int hist[256];
	double maxd = max;
	for (int i = 0; i < 256; i++)
	{
		hist[i] = cvRound(double(levels[i] / maxd) * 300);
		Point p1 = Point(i, 300 - hist[i]);
		Point p2 = Point(i, 300);
		line(histImage, p1, p2, Scalar(0), 1, 8, 0);
	}
	return histImage;
}

/*Contrast stretching function*/
Mat contrast_stretch(Mat img, double a, double b, double c, double d) {
	double m1 = c / a;
	double m2 = (d - c) / (b - a);
	double m3 = (255 - d) / (255 - b);

	int h = img.rows;
	int w = img.cols;
	Mat enhancedImg = img;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int x = img.at<uchar>(i, j);
			if (x < a)
			{
				enhancedImg.at<uchar>(i, j) = cvRound(double(m1*x));
			}
			else if (x >= a && x < b) {
				enhancedImg.at<uchar>(i, j) = cvRound(double(m2*(x - a) + c));
			}
			else {
				enhancedImg.at<uchar>(i, j) = cvRound(double(m3*(x - b) + d));
			}
		}
	}
	return enhancedImg;
}

