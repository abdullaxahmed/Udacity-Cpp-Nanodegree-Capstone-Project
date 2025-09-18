#include <vector>

#include "video_processing.h"

#ifndef DETECTOR_H
#define DETECTOR_H

class Segmentation {
    public:
        explicit Segmentation(ColorConverter& src);
        const cv::Mat& getForegroundFrame() const;
        const cv::Mat& getRefinedFrame() const;
        void RemoveBackground();
        void RefineMask();

    private:
        ColorConverter& _src;
        cv::Mat _fgMask;
        // Pointer to MOG2 background subtractor
        cv::Ptr<cv::BackgroundSubtractorMOG2> _mog2; 
        cv::Mat _refinedMask;
};

class ContourDetection {
    public:
        explicit ContourDetection(Segmentation& src);
        const std::vector<std::vector<cv::Point>>& getContours() const;
        const cv::Mat& getDrawing() const;
        void FindContours();

    private:
        Segmentation& _src;
        std::vector<std::vector<cv::Point>> _contours;
        // 2D vector for storing all detected contours
        
        cv::Mat _drawing;
        
};

#endif