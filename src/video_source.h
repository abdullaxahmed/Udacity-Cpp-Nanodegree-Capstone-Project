#include <opencv2/opencv.hpp>
#include <string>

#ifndef VIDEO_SOURCE_H
#define VIDEO_SOURCE_H

class videoProcess {
    public:

        explicit videoProcess(std::string  path) : _path(path) {
            _cap.open(path);    // Open input video file
        } 

        cv::VideoCapture& getCap() {return _cap;}
        void setCap(const std::string& path) {
            _cap.open(path);
            _path = path;
            }

        const cv::Mat& getFrame() const { return _frame; }
        const std::string& getPath() const { return _path; }

        bool readFrame();

    private:
        cv::VideoCapture _cap;  // Manages video stream
        cv::Mat _frame;         // Matrix object to hold a single video frame
        std::string _path; 
};

#endif