#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void img_crop(const std::string& img_path, const fs::path& cropped_save_path);