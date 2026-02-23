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
        std::cerr << "Error: Directory 'F2' does not exist." << std::endl;
        return 1;
    }

    if (fs::exists(cropped_save_dir)) {
        fs::remove_all(cropped_save_dir);
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
            continue;
        }

        img_crop(img_path, cropped_path);

        int score1 = 0;
        int score2 = 0;
        int left_count = 0;
        int right_count = 0;

        int total_imgs = std::distance(fs::directory_iterator(cropped_path), fs::directory_iterator{});
        int rounds = total_imgs / 2;

        for (int i = 1; i <= rounds; i++) {
            fs::path base = cropped_save_dir;

            fs::path left_path = cropped_save_dir / ("left_half_" + std::to_string(i) + ".png");
            cv::Mat mat1 = cv::imread(left_path.string());
            std::string player1_sign = img_classify(mat1);

            fs::path right_path = cropped_save_dir / ("right_half_" + std::to_string(i) + ".png");
            cv::Mat mat2 = cv::imread(right_path.string());
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

        fs::remove_all(cropped_save_dir);
        fs::create_directories(cropped_save_dir);
    }

    fs::remove_all(cropped_save_dir);

    return 0;
}