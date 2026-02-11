#include <iostream>
#include <filesystem>
#include <string>
#include "img_crop.h"
#include "img_classify.h"
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

		int player1_score = 0;
		int player2_score = 0;

		int score1 = 0;
		int score2 = 0;
		int count = std::distance(fs::directory_iterator(cropped_save_dir), fs::directory_iterator{});
		for (int i = 1; i <= count / 2; i++) {
			fs::path fs_cropped_path(cropped_path);

			fs::path fs_str1 = fs_cropped_path / ("left_half_" + std::to_string(i) + ".png");
			std::string str1 = fs_str1.string();
			cv::Mat mat1(cv::imread((str1)));
			std::string player1_sign = img_classify(mat1);

			fs::path fs_str2 = fs_cropped_path / ("right_half_" + std::to_string(i) + ".png");
			std::string str2 = fs_str2.string();
			cv::Mat mat2(cv::imread((str2)));
			std::string player2_sign = img_classify(mat2);

			if (player1_sign == "Rock" && player2_sign == "Paper") {
				score2++;
			}
			else if (player1_sign == "Rock" && player2_sign == "Scissors") {
				score1++;
			}
			else if (player1_sign == "Paper" && player2_sign == "Scissors") {
				score2++;
			}
			else if (player1_sign == "Paper" && player2_sign == "Rock") {
				score1++;
			}
			else if (player1_sign == "Scissors" && player2_sign == "Rock") {
				score2++;
			}
			else if (player1_sign == "Scissors" && player2_sign == "Paper") {
				score1++;
			}
		}
		if (score1 > score2) {
			std::cout << "Player1 won!\n";
		}
		else if (score1 < score2) {
			std::cout << "Player2 won!\n";
		}
		else {
			std::cout << "The game ended in a draw\n";
		}
	}

	fs::remove_all(cropped_save_dir);
	if (fs::exists(cropped_save_dir)) {
		std::cerr << "Error: Could not remove directory 'Cropped'." << std::endl;
		return 1;
	}

	return 0;

}
