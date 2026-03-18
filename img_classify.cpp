#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "img_classify.h"
#include <string>



Sign img_classify(const cv::Mat& img) {
	constexpr int SATURATION_THRESHOLD = 15;
	constexpr int PAPER_BRIGHTNESS_THRESHOLD = 200;

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
	if (diff < SATURATION_THRESHOLD) {
		if (brightness > PAPER_BRIGHTNESS_THRESHOLD) {
			return Sign::Paper;
		}
		else {
			return Sign::Rock;
		}
	}
	return Sign::Scissors;
}
