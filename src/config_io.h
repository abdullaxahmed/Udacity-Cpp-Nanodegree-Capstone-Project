#include <string>
#include <fstream>
#include <chrono>

#ifndef CONFIG_H
#define CONFIG_H

struct Params {
    std::string video;
    int         history;
    double      thresh;
    double      minarea;
    bool        shadows;
    std::string log;
};

Params loadParams(std::string filename);


class Logger {
public:
    explicit Logger(const std::string& path);
    ~Logger();
    bool ok() const;
    void write(int frame, int objects, double fps);

private:
    std::ofstream file;
};


class FpsMeter {
public:
    FpsMeter();
    double sample(); 

private:
    std::chrono::steady_clock::time_point last;
};

#endif