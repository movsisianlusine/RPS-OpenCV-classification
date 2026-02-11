#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "img_classify.h"
#include <string>


std::string img_classify(const cv::Mat& img) {
	cv::Scalar avg = cv::mean(img);

	double r = avg[0];
	double g = avg[1];
	double b = avg[2];

	//saturation
	double maxVal = std::max({ r, g, b });
	double minVal = std::min({ r, g, b });
	double diff = maxVal - minVal;

	double brightness = (r + g + b) / 3.0;

	//diff < 15 -> grey/white
	if (diff < 15) {
		if (brightness > 200) {
			return "Paper";
		}
		else {
			return "Rock";
		}
	}
	else {
		return "Scissors";
	}
}