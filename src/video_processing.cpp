#include "video_processing.h"
#include "display.h"
#include "detector.h"
#include "config_io.h" 

VideoReader::VideoReader(const std::string& path) : _path(path) {
    _cap.open(path);
}

VideoReader::~VideoReader() {
    if (_cap.isOpened()) {
        _cap.release();
    }
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

ColorConverter::ColorConverter(std::shared_ptr<VideoReader> src) : _src(std::move(src)) {};

const cv::Mat& ColorConverter::getGrayFrame() const {
    return _grayFrame;
}

void ColorConverter::ConvertFrameColor() {
    const cv::Mat& src = _src->getFrame();
    cvtColor(src, _grayFrame, cv::COLOR_BGR2GRAY);  // Convert BGR frames to grayscale 
}


void processVideo(VideoReader& videoReader,
                  ColorConverter& colorConverter,
                  Segmentation& segmentation,
                  ContourDetection& contourDetection,
                  ContourFeatures& contourFeatures,
                  Display& display,
                  Logger& logger)
{
    FpsMeter fps;
    int frameIndex = 0;

    while (videoReader.readFrame()) {
        colorConverter.ConvertFrameColor();
        segmentation.RemoveBackground();
        segmentation.RefineMask();
        contourDetection.FindContours();
        contourFeatures.ExtractFeatures();
        display.renderBoxes();

        double f = fps.sample();
        logger.write(frameIndex, contourDetection.getCount(), f);
        frameIndex++;

        int key = display.show();
        if (key == 27) break; // ESC to exit
    }
}