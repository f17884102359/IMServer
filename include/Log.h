#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <ctime>
#include "Config.h"

enum Level {
    Debug,
    Info,
    Notice,
    Warning,
    Error,
    Critical,
    Alert,
    Emergency
};

// 声明静态成员变量（不要在头文件中定义）
extern std::ofstream output_file_;
extern std::mutex mutex_;

class Log {
public:
    Log(Level level, const char* file, const char* function, int line) 
        : level_(level) {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_ << getCurrentTime() << " | " << file << " | " << function << " | " << line << " | ";
    }

    template<typename T>
    Log& operator<<(const T& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ostringstream oss;
        oss << msg;
        buffer_ << oss.str();
        return *this;
    }

    ~Log() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (output_file_.is_open()) {
            output_file_ << buffer_.str() << std::endl;
        } else {
            std::cout << buffer_.str() << std::endl;
        }
    }

    static void setOutputFile(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (output_file_.is_open()) {
            output_file_.close();
        }
        output_file_.open(filename, std::ios::out | std::ios::app);
    }

private:
    Level level_;
    std::ostringstream buffer_;

    std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }
};

#define LOG(level) Log(level, __FILE__, __FUNCTION__, __LINE__)

#endif // LOG_H
