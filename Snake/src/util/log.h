#ifndef SNAKE_UTIL_LOG_H_
#define SNAKE_UTIL_LOG_H_
#include <fstream>
#include <string>

namespace snake {

class Logger {

    void log(std::string str);
    void closeLog();
private:
    std::ofstream myfile;
};

void log(std::string str);

} // namespace snake

#endif // !SNAKE_UTIL_LOG_H_
