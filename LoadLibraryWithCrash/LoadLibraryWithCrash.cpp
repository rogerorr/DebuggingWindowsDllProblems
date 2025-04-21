#include <Windows.h>
#include <iostream>

int main() {
  HMODULE h = ::LoadLibrary("CrashingDll");
  if (!h) {
    std::cerr << "LoadLibrary failed with error: " << GetLastError() << '\n';
  }
}