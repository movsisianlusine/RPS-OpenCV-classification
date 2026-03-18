#include <iostream>
#include <filesystem>
#include <string>
#include "img_crop.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "CLI/CLI11.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	CLI::App app{ "RPS arguments parser" };

	std::vector<std::pair<std::string, std::string>> dirs;

	app.add_option("--rps", dirs, "Directory followed by type (simple/complex)") 
		->expected(-1);
   	
	fs::path cropped_save_dir = fs::path(PROJECT_ROOT_DIR) / "Cropped";

	CLI11_PARSE(app, argc, argv);

	for (const auto& [path, type] : dirs) {
		fs::path images_dir = fs::path(PROJECT_ROOT_DIR) / path;
		if (!fs::exists(images_dir)) {
			throw std::runtime_error("Error: Directory" + path + " does not exist at " + images_dir.string());
		}

		if (fs::exists(cropped_save_dir)) {
			fs::remove_all("Cropped");
		}
		fs::create_directories(cropped_save_dir);
		if (!fs::exists(cropped_save_dir)) {
			throw std::runtime_error("Error: Could not create directory 'Cropped' at " + cropped_save_dir.string());
		}
		std::string cropped_path = cropped_save_dir.string();

		for (const auto& entry : fs::directory_iterator(images_dir)) {
			std::string img_path = entry.path().string();
			if (!fs::exists(img_path)) {
				throw std::runtime_error("Error: Image file does not exist at " + img_path);
			}

			ImageCropper* cropper;
			if (type == "simple") {
				cropper = new SimpleImageCropper();
			}
			else {
				cropper = new ComplexImageCropper();
			}
			cropper->img_crop(img_path, cropped_path);
		}
	}
	
	//fs::remove_all(cropped_save_dir);
	//if (fs::exists(cropped_save_dir)) {
	//	std::cerr << "Error: Could not remove directory 'Cropped'." << std::endl;
	//	return 1;
	//}

}