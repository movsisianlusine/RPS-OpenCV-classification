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

    fs::path images_dir = fs::path(PROJECT_ROOT_DIR) / "F2";
    fs::path cropped_save_dir = fs::path(PROJECT_ROOT_DIR) / "Cropped";

    if (!fs::exists(images_dir)) {
        throw std::runtime_error("Input directory F2 does not exist");
    }

    if (fs::exists(cropped_save_dir)) {
        fs::remove_all(cropped_save_dir);
    }

    fs::create_directories(cropped_save_dir);

    if (!fs::exists(cropped_save_dir)) {
        throw std::runtime_error("Could not create directory 'Cropped");
    }

    std::string cropped_path = cropped_save_dir.string();

    for (const auto& entry : fs::directory_iterator(images_dir)) {
        std::string img_path = entry.path().string();
        if (!fs::exists(img_path)) {
			throw std::runtime_error("File does not exist: " + img_path);
        }

        img_crop(img_path, cropped_path);

        int score1 = 0;
        int score2 = 0;
        int left_count = 0;
        int right_count = 0;

        int total_imgs = std::distance(fs::directory_iterator(cropped_path), fs::directory_iterator{});
        int rounds = total_imgs / 2;

        constexpr const char* LEFT_PREFIX = "left_half_";
        constexpr const char* RIGHT_PREFIX = "right_half_";
        constexpr const char* EXT = ".png";

        for (int i = 1; i <= rounds; i++) {
            fs::path base = cropped_save_dir;

            fs::path left_path = cropped_save_dir / (std::string(LEFT_PREFIX) + std::to_string(i) + EXT);
            cv::Mat mat1 = cv::imread(left_path.string());
            Sign player1_sign = img_classify(mat1);

			fs::path right_path = cropped_save_dir / (std::string(RIGHT_PREFIX) + std::to_string(i) + EXT);
            cv::Mat mat2 = cv::imread(right_path.string());
            Sign player2_sign = img_classify(mat2);

            constexpr const char* ROCK = "Rock";
            constexpr const char* PAPER = "Paper";
            constexpr const char* SCISSORS = "Scissors";

            if (player1_sign == Sign::Rock && player2_sign == Sign::Paper) {
                score2++;
            }
            else if (player1_sign == Sign::Rock && player2_sign == Sign::Scissors) {
                score1++;
            }
            else if (player1_sign == Sign::Paper && player2_sign == Sign::Scissors) {
                score2++;
            }
            else if (player1_sign == Sign::Paper && player2_sign == Sign::Rock) {
                score1++;
            }
            else if (player1_sign == Sign::Scissors && player2_sign == Sign::Rock) {
                score2++;
            }
            else if (player1_sign == Sign::Scissors && player2_sign == Sign::Paper) {
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

        fs::remove_all(cropped_save_dir);
        fs::create_directories(cropped_save_dir);
    }

    fs::remove_all(cropped_save_dir);

    return 0;
}