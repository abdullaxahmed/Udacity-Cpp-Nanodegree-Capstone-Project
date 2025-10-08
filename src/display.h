#include "detector.h"

#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
public:
    Display(VideoReader& videoReader, ColorConverter& colorConverter, Segmentation& segmentation,
            ContourDetection& contourDetection, ContourFeatures& contourFeatures);
    
    ~Display();

    // Don't allow display copy

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    void renderBoxes();
    int  show();
    const cv::Mat& frame() const;

private:
    VideoReader&      videoReader_;
    ColorConverter&   colorConverter_;
    Segmentation&     segmentation_;
    ContourDetection& contourDetection_;
    ContourFeatures&  contourFeatures_;

    cv::Mat boxedFrame_;
};

#endif