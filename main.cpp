#include <iostream>
#include <filesystem>
#include <string>
#include "img_crop.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main() {
	fs::path images_dir = fs::path(PROJECT_ROOT_DIR) / "F1";
	fs::path cropped_save_dir = fs::path(PROJECT_ROOT_DIR) / "Cropped";
	if (!fs::exists(images_dir)) {
		std::cerr << "Error: Directory 'F1' does not exist." << std::endl;
		return 1;
	}

	if (fs::exists(cropped_save_dir)) {
		fs::remove_all("Cropped");
	}

	fs::create_directories(cropped_save_dir);
	if (!fs::exists(cropped_save_dir)) {
		std::cerr << "Error: Could not create directory 'Cropped'." << std::endl;
		return 1;
	}

	std::string cropped_path = cropped_save_dir.string();

	for (const auto& entry : fs::directory_iterator(images_dir)) {
		std::string img_path = entry.path().string();
		if (!fs::exists(img_path)) {
			std::cerr << "File does not exist: " << img_path << std::endl;
		}
		img_crop(img_path, cropped_path);

		fs::remove_all(cropped_save_dir);
		if (fs::exists(cropped_save_dir)) {
			std::cerr << "Error: Could not remove directory 'Cropped'." << std::endl;
			return 1;
		}
		return 0;
	}
}