#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void img_crop(std::string img_path, fs::path cropped_save_path);