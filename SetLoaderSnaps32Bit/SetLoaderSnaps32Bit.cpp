#include <windows.h>

#include <imagehlp.h>
#include <iostream>
#include <stdexcept>
#include <string>

#pragma comment(lib, "Imagehlp.lib")

static const int SHOW_LOADER_SNAPS = 2;

struct LoadedImage : public LOADED_IMAGE {
  LoadedImage(const std::string &filename) {
    if (!MapAndLoad(filename.c_str(), nullptr, this, false, false)) {
      throw std::runtime_error("MapAndLoad(" + filename +
                               ") failed: " + std::to_string(GetLastError()));
    }
  }

  ~LoadedImage() {
    if (!UnMapAndLoad(this)) {
      std::cerr << "UnMapAndLoad failed: " << GetLastError() << '\n';
    }
  }
};

void UpdateImageConfigInformation(const std::string &filename) {
  LoadedImage loadedImage{filename};
  std::cout << "Mapped: " << loadedImage.ModuleName << '\n';

  IMAGE_LOAD_CONFIG_DIRECTORY imageConfig = {sizeof(imageConfig)};
  if (!GetImageConfigInformation(&loadedImage, &imageConfig)) {
    throw std::runtime_error("GetImageConfigInformation(" +
                             std::to_string(sizeof(imageConfig)) +
                             ") failed: " + std::to_string(GetLastError()));
  }

  if (imageConfig.GlobalFlagsSet & SHOW_LOADER_SNAPS) {
    std::cout << "Show Loader Snaps flag already set in image\n";
  } else {
    imageConfig.GlobalFlagsSet |= SHOW_LOADER_SNAPS;

    if (!SetImageConfigInformation(&loadedImage, &imageConfig)) {
      throw std::runtime_error("SetImageConfigInformation failed: " +
                               std::to_string(GetLastError()));
    }
    std::cout << "Set Show Loader Snaps flag\n";
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Syntax: " << argv[0] << " <filename>\n";
    return 1;
  }

  try {
    UpdateImageConfigInformation(argv[argc - 1]);
  } catch (const std::exception &ex) {
    std::cerr << "Unexpected error: " << ex.what() << '\n';
    return 1;
  }
}
