#include <iostream>
#include <filesystem>
#include <string>
#include "img_crop.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	
	fs::path cropped_save_dir = fs::path(PROJECT_ROOT_DIR) / "Cropped";

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		
		if (arg == "--rps") {
			if (i + 2 >= argc) {
				throw std::runtime_error("Error: --rps option must have 2 arguments: 'path' and 'type'");
			}

			std::string path;
			std::string type;
			if (i + 2 < argc) {
				path = argv[i + 1];
				type = argv[i + 2];
			}
			if (type != "simple" && type != "complex") {
				throw std::runtime_error("Error: Invalid image type: " + type + ". The type should be either 'simple' or 'complex'");
			}

			std::cout << "Processing images in directory: " << path << " with type: " << type << std::endl;
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
			i += 2;
		}
	}
	
	fs::remove_all(cropped_save_dir);
	if (fs::exists(cropped_save_dir)) {
		std::cerr << "Error: Could not remove directory 'Cropped'." << std::endl;
		return 1;
	}

}