#include <stdexcept>

namespace {
struct Bad {
  Bad() { throw std::runtime_error("Bad things happened"); }
} bad_initialization;
} // namespace

__declspec(dllexport) void exportedSymbol() {}
