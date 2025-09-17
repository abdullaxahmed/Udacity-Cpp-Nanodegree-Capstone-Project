#include "detector.h"

Segmentation::Segmentation(ColorConverter& src) 
    : _src(src), _fgMask(), _refinedMask(), _mog2(cv::createBackgroundSubtractorMOG2(500,500,false)) {};
// cv::createBackgroundSubtractorMOG2 (int history, double varThreshold, bool detectShadows);


const cv::Mat& Segmentation::getForegroundFrame() const {
    return _fgMask;
}
const cv::Mat& Segmentation::getRefinedFrame() const {
    return _refinedMask;
}

void Segmentation::RemoveBackground() {
    const cv::Mat& src = _src.getGrayFrame();
    _mog2->apply(src,_fgMask);  // apply MOG2 (Gaussian Mixture-based Background/Foreground Segmentation Algorithm)
}

void Segmentation::RefineMask () {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(2, 2));
    // cv::getStructuringElement ( shape, kernel size)
    // Create a small cross-shaped structuring element

    cv::erode(_fgMask, _refinedMask, kernel);
    // cv::erode( cv::Mat src, cv::Mat erosion_dst, cv::Mat element );
    // Erode mask to remove small white noise points from the foreground mask
}
