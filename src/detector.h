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
        cv::Ptr<cv::BackgroundSubtractorMOG2> _mog2;
        cv::Mat _refinedMask;
};

#endif