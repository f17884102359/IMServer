#include "Log.h"

int main() {
    // Set the output file for logging
    Log::setOutputFile(LOGFILE);

    // Log messages with different levels
    LOG(Level::Debug) << "This is a debug message.";
    LOG(Info) << "This is an info message.";
    LOG(Notice) << "This is a notice message.";
    LOG(Warning) << "This is a warning message.";
    LOG(Error) << "This is an error message.";
    LOG(Critical) << "This is a critical message.";
    LOG(Alert) << "This is an alert message.";
    LOG(Emergency) << "This is an emergency message.";

    return 0;
}