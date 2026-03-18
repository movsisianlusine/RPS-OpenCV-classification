#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


namespace fs = std::filesystem;

class ImageCropper {
public:
	virtual void img_crop(const std::string& img_path, const fs::path& cropped_save_path) const = 0;
	std::vector<bool> ImageCropper::get_contains_obj(const cv::Mat& img) const;
	virtual ~ImageCropper() = default;
};

class SimpleImageCropper : public ImageCropper{
public:
	void img_crop(const std::string& img_path, const fs::path& cropped_save_path) const override;
};

class ComplexImageCropper : public ImageCropper {
public:
	void img_crop(const std::string& img_path, const fs::path& cropped_save_dir) const override;
};

