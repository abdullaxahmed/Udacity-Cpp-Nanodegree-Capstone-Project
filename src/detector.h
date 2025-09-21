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


class ContourFeatures {
public:
    explicit ContourFeatures(ContourDetection& src);
    const std::vector<cv::Rect>& getBoundingBoxes() const;
    const std::vector<double>& getContourAreas() const;
    const std::vector<cv::Point2d>& getCentroids() const;
    const std::vector<double>& getAspectRatio() const;
    const std::vector<double>& getFillRatio() const;

    void ExtractFeatures();

private:
    ContourDetection& _src;
    std::vector<cv::Rect> _boxes;
    // vector for storing Rectangles (Bounding Boxes)
    std::vector<double> _contourArea;
    std::vector<cv::Point2d> _centroids;
    std::vector<double> _aspectRatio;
    std::vector<double> _fillRatio;
};

#endif