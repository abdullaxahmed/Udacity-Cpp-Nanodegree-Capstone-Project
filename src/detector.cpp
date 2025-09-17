#include "detector.h"

Segmentation::Segmentation(ColorConverter& src) 
    : _src(src), _fgMask(), _mog2(cv::createBackgroundSubtractorMOG2(500,500,false)) {};

const cv::Mat& Segmentation::getForegroundFrame() const {
    return _fgMask;
}

void Segmentation::RemoveBackground() {
    const cv::Mat& src = _src.getGrayFrame();
    _mog2->apply(src,_fgMask);
}