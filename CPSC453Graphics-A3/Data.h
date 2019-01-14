#ifndef DATA_H_
#define DATA_H_

#include <vector>
#include <chrono>
#include <string>

class Data {
public:
    static int pointCount;
    static float scMaxLength;

    static float Q1_vertices1[][2];
    static float Q1_vertices2[][2];
    
    static std::string name;
    static std::string scrollText;


    static float scRate;
    static bool scEnabled;

    static float scLocation_x;
    static float scLocation_y;
    static std::chrono::time_point<std::chrono::system_clock> startTime;
    static std::chrono::time_point<std::chrono::system_clock> currentTime;

    static std::vector<std::string> bonus1Text;
    static int QuestionNumber;
};

#endif /* DATA_H_ */