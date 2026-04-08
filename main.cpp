#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "img_crop.h"
#include <opencv2/opencv.hpp>
#include <CLI/CLI.hpp>

namespace fs = std::filesystem;

struct ImageInput {
    std::string path;
    std::string type;
};

std::vector<std::vector<std::string>> arg_parser(int argc, char** argv) {
    CLI::App app{ "Image processor" };

    std::vector<std::vector<std::string>> image_args;

    app.add_option("--image", image_args,
        "Image directory and type (simple|complex)")
        ->expected(2)
        ->multi_option_policy(CLI::MultiOptionPolicy::TakeAll);

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e) {
        app.exit(e);
        throw std::runtime_error("Could not parse through command line arguments");
    }

    return image_args;
}

int main(int argc, char** argv) {
    std::vector<std::vector<std::string>> image_args = arg_parser(argc, argv);

    std::vector<ImageInput> images;

    for (const auto& entry : image_args) {
        if (entry.size() != 2) {
            throw std::runtime_error("Each --image requires: <path> <simple|complex>");
        }

        if (entry[1] != "simple" && entry[1] != "complex") {
            throw std::runtime_error("Invalid image type: " + entry[1]);
        }

        images.push_back({ entry[0], entry[1] });
    }

    fs::path cropped_save_dir = fs::path(PROJECT_ROOT_DIR) / "Cropped";
    if (fs::exists(cropped_save_dir)) {
        fs::remove_all(cropped_save_dir);
    }

    fs::create_directories(cropped_save_dir);
    if (!fs::exists(cropped_save_dir)) {
        throw std::runtime_error("Could not create Cropped directory at " + cropped_save_dir.string());
    }

    for (const auto& img : images) {
        fs::path images_dir = fs::path(PROJECT_ROOT_DIR) / img.path;
        if (!fs::exists(images_dir)) {
            throw std::runtime_error("Directory does not exist: " + images_dir.string());
        }

        for (const auto& entry : fs::directory_iterator(images_dir)) {
            std::string img_path = entry.path().string();
            if (!fs::exists(img_path)) {
                throw std::runtime_error("Image does not exist: " + img_path);
            }

            std::unique_ptr<ImageCropper> cropper;
            if (img.type == "simple") {
                cropper = std::make_unique<SimpleImageCropper>();
            }
            else {
                cropper = std::make_unique<ComplexImageCropper>();
            }

            cropper->img_crop(img_path, cropped_save_dir.string());

            fs::remove_all(cropped_save_dir);
            if (fs::exists(cropped_save_dir)) {
                throw std::runtime_error("Error: Could not remove directory 'Cropped'.");
            }
        }
    }

    return 0;
}