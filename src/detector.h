#include "video_processing.h"

#ifndef DETECTOR_H
#define DETECTOR_H

class Segmentation {
    public:
        explicit Segmentation(ColorConverter& src);
        const cv::Mat& getForegroundFrame() const;
        void RemoveBackground();

    private:
        ColorConverter& _src;
        cv::Mat _fgMask;
        cv::Ptr<cv::BackgroundSubtractorMOG2> _mog2;
};

#endif