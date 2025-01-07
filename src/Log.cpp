#include "Log.h"

// 在 Log.cpp 中定义静态成员变量（仅定义一次）
std::ofstream output_file_;
std::mutex mutex_;
