#include <Windows.h>
#include <iostream>

int main() {
  HMODULE h = ::LoadLibrary("MissingDll");
  if (!h) {
    std::cerr << "LoadLibrary failed with error: " << GetLastError() << '\n';
  }
}