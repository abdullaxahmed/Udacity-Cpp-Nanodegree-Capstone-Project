#include <opencv2/opencv.hpp>
#include <string>
#include <memory>

#ifndef VIDEO_PROCESSING_H
#define VIDEO_PROCESSING_H

class Segmentation;
class ContourDetection;
class ContourFeatures;
class Display;
class Logger;

class VideoReader {
    public:

        explicit VideoReader(const std::string& path);
        VideoReader() = default;
        ~VideoReader();

        cv::VideoCapture& getCap();
        void setCap(const std::string& path);

        const cv::Mat& getFrame() const;
        const std::string& getPath() const;

        bool readFrame();

    private:
        cv::VideoCapture _cap;  // Manages video stream
        cv::Mat _frame;         // Matrix object to hold a single video frame
        std::string _path; 
};

class ColorConverter {
    public:
        explicit ColorConverter(std::shared_ptr<VideoReader> src);
        const cv::Mat& getGrayFrame() const;
        void ConvertFrameColor();

    private:
        std::shared_ptr<VideoReader> _src;
        cv::Mat _grayFrame;
};

void processVideo(VideoReader& videoReader,
                  ColorConverter& colorConverter,
                  Segmentation& segmentation,
                  ContourDetection& contourDetection,
                  ContourFeatures& contourFeatures,
                  Display& display,
                  Logger& logger);

#endif