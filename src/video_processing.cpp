#include "video_processing.h"

VideoReader::VideoReader(const std::string& path) : _path(path) {
    _cap.open(path);
}

cv::VideoCapture& VideoReader::getCap() {
    return _cap;
}

void VideoReader::setCap(const std::string& path) {
    _cap.open(path);    // Open input video file
    _path = path;
}

const cv::Mat& VideoReader::getFrame() const {
    return _frame;
}

const std::string& VideoReader::getPath() const {
    return _path;
}

bool VideoReader::readFrame() {
    return _cap.read(_frame);
}

ColorConverter::ColorConverter(VideoReader& src) : _src(src) {};

const cv::Mat& ColorConverter::getGrayFrame() const {
    return _grayFrame;
}

void ColorConverter::ConvertFrameColor() {
    const cv::Mat& src = _src.getFrame();
    cvtColor(src, _grayFrame, cv::COLOR_BGR2GRAY);  // Convert BGR frames to grayscale 
}