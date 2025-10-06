#include "config_io.h"
#include <iostream>
#include <fstream>

/*  Read parameters from text file implementation */

Params loadParams(std::string filename) {
    Params p;

    // default values
    p.video = "../videos/highway.mp4";
    p.history = 800;
    p.thresh = 80.0;
    p.minarea = 500.0;
    p.shadows = true;
    p.log = "run_log.csv";

    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << "Could not open " << filename << ". Using default values.\n";
        return p;
    }

    std::string key, value;
    while (file >> key) {
        int pos = key.find('=');
        if (pos == -1) continue;

        value = key.substr(pos + 1);
        key = key.substr(0, pos);

        if (key == "video") p.video = value;
        else if (key == "history") p.history = atoi(value.c_str());
        else if (key == "thresh") p.thresh = atof(value.c_str());
        else if (key == "minarea") p.minarea = atof(value.c_str());
        else if (key == "shadows") p.shadows = (value == "1");
        else if (key == "log") p.log = value;
    }

    file.close();
    std::cout << "Parameters loaded successfully from " << filename << "\n";
    return p;
}

/*  CSV logging implementation */

Logger::Logger(const std::string& path) {
    file.open(path.c_str());
    if (file.is_open()) {
        file << "frame,objects,fps\n";
    } else {
        std::cout << "WARN: could not open log file: " << path << "\n";
    }
}

Logger::~Logger() {
    if (file.is_open()) file.close();
}

bool Logger::ok() const {
    return file.is_open();
}

void Logger::write(int frame, int objects, double fps) {
    if (file.is_open()) {
        file << frame << "," << objects << "," << fps << "\n";
    }
}

/*  FPS counting implementation */

FpsMeter::FpsMeter() {
    last = std::chrono::steady_clock::now();
}

double FpsMeter::sample() {
    auto now = std::chrono::steady_clock::now();
    double secs = std::chrono::duration<double>(now - last).count();
    last = now;
    if (secs <= 0.0) return 0.0;
    return 1.0 / secs;
}