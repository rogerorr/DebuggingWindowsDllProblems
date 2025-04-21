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
      throw std::runtime_error(std::string("MapAndLoad(") + filename +
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

  // Get the PE header.
  auto *pNtHeader = loadedImage.FileHeader;

  // Is this a real PE image?
  if ((IsBadReadPtr(pNtHeader, sizeof(IMAGE_NT_HEADERS))) ||
      (IMAGE_NT_SIGNATURE != pNtHeader->Signature)) {
    throw std::runtime_error("Not a valid executable image");
  }

  auto *DataDirectory =
      (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
          ? ((PIMAGE_NT_HEADERS32)pNtHeader)->OptionalHeader.DataDirectory
          : ((PIMAGE_NT_HEADERS64)pNtHeader)->OptionalHeader.DataDirectory;

  // Get the pointer to the load config section.
  DWORD const vaLoadConfig =
      DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress;
  // Does it have a load config?
  if (!vaLoadConfig) {
    throw std::runtime_error("No load config section");
  }

  auto *pLoadConfig = (PIMAGE_LOAD_CONFIG_DIRECTORY)ImageRvaToVa(
      pNtHeader, loadedImage.MappedAddress, vaLoadConfig, nullptr);

  if (pLoadConfig->GlobalFlagsSet & SHOW_LOADER_SNAPS) {
    std::cout << "Show Loader Snaps flag already set in image\n";
  } else {
    pLoadConfig->GlobalFlagsSet |= SHOW_LOADER_SNAPS;
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
