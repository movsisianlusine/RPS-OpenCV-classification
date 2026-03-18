#pragma once 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>

enum class Sign {
	Rock,
	Paper,
	Scissors
}; 

Sign img_classify(const cv::Mat& img);

