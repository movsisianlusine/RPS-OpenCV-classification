#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "img_crop.h"
#include <filesystem>

namespace fs = std::filesystem;

void img_crop(std::string img_path, fs::path cropped_save_path) {
	if (!cropped_save_path.empty()) {
		for (const auto& dir : fs::directory_iterator(cropped_save_path)) {
			fs::remove_all(dir.path());
		}
	}

	cv::Mat img(cv::imread(img_path));
	if (img.empty()) {
		std::cerr << "Error: Could not open or find the image at " << img_path << std::endl;
		return;
	}

	int height = img.rows;
	int width = img.cols;

	std::vector<bool> containsObj(height, false);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
			if (pixel[0] < 240 || pixel[1] < 240 || pixel[2] < 240) {
				containsObj[y] = true;
				break;
			}
		}
	}

	int startY = 0;
	int endY = 0;
	bool inObj = false;
	int round = 1;

	for (int y = 0; y < height; y++) {
		if (containsObj[y] && !inObj) {
			startY = y;
			inObj = true;
		}

		if ((!containsObj[y] || y == height - 1) && inObj) {
			endY = y;

			if (endY - startY < 20) {
				inObj = false;
				continue;

			}
			cv::Rect left_sign(0, startY, width / 2, endY - startY);
			cv::Rect right_sign(width / 2, startY, width / 2, endY - startY);

			cv::Mat left_img = img(left_sign);
			cv::Mat right_img = img(right_sign);

			std::string left_half = "left_half_" + std::to_string(round) + ".png";
			std::string right_half = "right_half_" + std::to_string(round) + ".png";

			fs::path left_path = cropped_save_path / left_half;
			fs::path right_path = cropped_save_path / right_half;

			cv::imwrite(left_path.string(), left_img);
			cv::imwrite(right_path.string(), right_img);

			round++;
			inObj = false;
		}
	}
}