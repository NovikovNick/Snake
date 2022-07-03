#include "log.h"


namespace snake {

void Logger::log(std::string str) {
    /*myfile.open("E:\\WORKSPACE\\LOG\\log.txt", std::ios_base::app);
    myfile << str << "\n";*/
}
void Logger::closeLog() {
   //myfile.close();
}


void log(std::string str) {
    /*std::ofstream myfile;
    myfile.open("E:\\WORKSPACE\\LOG\\log.txt", std::ios_base::app);
    myfile << str << "\n";
    myfile.close();*/
}

} // namespace snake
