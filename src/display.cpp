#include "display.h"

Display::Display(VideoReader& videoReader,
                 ColorConverter& colorConverter,
                 Segmentation& segmentation,
                 ContourDetection& contourDetection,
                 ContourFeatures& contourFeatures)
    : videoReader_(videoReader),
      colorConverter_(colorConverter),
      segmentation_(segmentation),
      contourDetection_(contourDetection),
      contourFeatures_(contourFeatures) {}

void Display::renderBoxes() {
    boxedFrame_ = videoReader_.getFrame().clone();
    for (const auto& box : contourFeatures_.getBoundingBoxes()) {
        cv::rectangle(boxedFrame_, box, cv::Scalar(0, 0, 255), 2);
    }
}

int Display::show() {
    cv::imshow("Detections", boxedFrame_);
    return cv::waitKey(10);
}

const cv::Mat& Display::frame() const {
    return boxedFrame_;
}