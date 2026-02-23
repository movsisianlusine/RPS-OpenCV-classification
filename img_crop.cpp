#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<bool> get_contains_obj(const cv::Mat& img) {
    const int height = img.rows;
    const int white_threshold = 255;
    std::vector<bool> contains_obj(height, false);

    for (int y = 0; y < height; y++) {
        const cv::Vec3b* row_begin = img.ptr<cv::Vec3b>(y);
        const cv::Vec3b* row_end = row_begin + img.cols;

        bool has_object = std::any_of(row_begin, row_end, [white_threshold](const cv::Vec3b& pixel) {
            return pixel[0] < white_threshold ||
                pixel[1] < white_threshold ||
                pixel[2] < white_threshold;
            });
        contains_obj[y] = has_object;
    }

    return contains_obj;
}

void img_crop(const std::string& img_path, const fs::path& cropped_save_path) {
    if (fs::exists(cropped_save_path)) {
        fs::remove_all(cropped_save_path);
    }
    fs::create_directories(cropped_save_path);

    cv::Mat img(cv::imread(img_path));
    if (img.empty()) {
        std::cerr << "Error: Could not open or find the image at " << img_path << std::endl;
        return;
    }

    cv::Mat left_half_img = img(cv::Rect(0, 0, img.cols / 2, img.rows));
    cv::Mat right_half_img = img(cv::Rect(img.cols / 2, 0, img.cols / 2, img.rows));

    const std::vector<bool> contains_obj_left = get_contains_obj(left_half_img);
    const std::vector<bool> contains_obj_right = get_contains_obj(right_half_img);

    int start_y_left = 0;
    int start_y_right = 0;
    bool in_obj_left = false;
    bool in_obj_right = false;
    int round_left = 1;
    int round_right = 1;
    const int min_height = 5;

    for (int y = 0; y < img.rows; y++) {
        if (contains_obj_left[y] && !in_obj_left) {
            start_y_left = y;
            in_obj_left = true;
        }
        if ((!contains_obj_left[y] || y == img.rows - 1) && in_obj_left) {
            int h = y - start_y_left;
            if (h >= min_height) {
                cv::Rect left_sign(0, start_y_left, img.cols / 2, h);
                std::string save_path = (cropped_save_path / ("left_half_" + std::to_string(round_left) + ".png")).string();
                cv::imwrite(save_path, img(left_sign));
                round_left++;
            }
            in_obj_left = false;
        }

        if (contains_obj_right[y] && !in_obj_right) {
            start_y_right = y;
            in_obj_right = true;
        }
        if ((!contains_obj_right[y] || y == img.rows - 1) && in_obj_right) {
            int h = y - start_y_right;
            if (y - start_y_right >= min_height) {
                cv::Rect right_sign(img.cols / 2, start_y_right, img.cols / 2, y - start_y_right);
                std::string save_path = (cropped_save_path / ("right_half_" + std::to_string(round_right) + ".png")).string();
                cv::imwrite(save_path, img(right_sign));
                round_right++;
            }
            in_obj_right = false;
        }
    }
}


//#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <string>
//#include <vector>
//#include "img_crop.h"
//#include <filesystem>
//
//namespace fs = std::filesystem;
//
//std::vector<bool> get_containsObj(const cv::Mat& img) {
//    const int height = img.rows;
//    const int white_threshold = 240;
//    std::vector<bool> contains_obj(height, false);
//
//    for (int y = 0; y < height; y++) {
//        const cv::Vec3b* row_begin = img.ptr<cv::Vec3b>(y);
//        const cv::Vec3b* row_end = row_begin + img.cols;
//
//        bool has_object = std::any_of(row_begin, row_end, [white_threshold](const cv::Vec3b& pixel) {
//            return pixel[0] < white_threshold ||
//                pixel[1] < white_threshold ||
//                pixel[2] < white_threshold;
//            });
//        contains_obj[y] = has_object;
//    }
//
//    return contains_obj;
//}
//
//void img_crop(const std::string& img_path, const fs::path& cropped_save_path) {
//    if (!cropped_save_path.empty()) {
//        for (const auto& dir : fs::directory_iterator(cropped_save_path)) {
//            fs::remove_all(dir.path());
//        }
//    }
//
//    cv::Mat img(cv::imread(img_path));
//    if (img.empty()) {
//        std::cerr << "Error: Could not open or find the image at " << img_path << std::endl;
//        return;
//    }
//
//    const int height = img.rows;
//    const int width = img.cols;
//    const std::vector<bool> containsObj = get_containsObj(img);
//    int start_y = 0;
//    int end_y = 0;
//    bool in_obj = false;
//    int round = 1;
//
//    for (int y = 0; y < height; y++) {
//        if (containsObj[y] && !in_obj) {
//            start_y = y;
//            in_obj = true;
//        }
//        if ((!containsObj[y] || y == height - 1) && in_obj) {
//            end_y = y;
//            const int minHeight = 20;
//            if (end_y - start_y < minHeight) {
//                in_obj = false;
//                continue;
//            }
//            cv::Rect left_sign(0, start_y, width / 2, end_y - start_y);
//            cv::Rect right_sign(width / 2, start_y, width / 2, end_y - start_y);
//
//            cv::Mat left_img = img(left_sign);
//            cv::Mat right_img = img(right_sign);
//
//            std::string left_half = "left_half_" + std::to_string(round) + ".png";
//            std::string right_half = "right_half_" + std::to_string(round) + ".png";
//
//            fs::path left_path = cropped_save_path / left_half;
//            fs::path right_path = cropped_save_path / right_half;
//
//            cv::imwrite(left_path.string(), left_img);
//            cv::imwrite(right_path.string(), right_img);
//
//            round++;
//            in_obj = false;
//        }
//    }
//}