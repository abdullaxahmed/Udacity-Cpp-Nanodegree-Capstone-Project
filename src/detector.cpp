#include "detector.h"

/*  Background Removal Imlementation  */

Segmentation::Segmentation(std::shared_ptr<ColorConverter> src, int history, double varThresh, bool detectShadows)
: _src(std::move(src)) {
    _mog2 = cv::createBackgroundSubtractorMOG2(history, varThresh, detectShadows);
}
// cv::createBackgroundSubtractorMOG2 (int history, double varThreshold, bool detectShadows);

Segmentation::~Segmentation() = default;

Segmentation::Segmentation(Segmentation&& other) noexcept
    : _src(std::move(other._src)), _fgMask(std::move(other._fgMask)),_mog2(std::move(other._mog2)),
      _refinedMask(std::move(other._refinedMask)) {}

Segmentation& Segmentation::operator=(Segmentation&& other) noexcept {
    if (this != &other) {
        _src = std::move(other._src);
        _fgMask = std::move(other._fgMask);
        _mog2 = std::move(other._mog2);
        _refinedMask = std::move(other._refinedMask);
    }
    return *this;
}

const cv::Mat& Segmentation::getForegroundFrame() const { return _fgMask; }
const cv::Mat& Segmentation::getRefinedFrame() const { return _refinedMask; }

void Segmentation::RemoveBackground() {
    const cv::Mat& src = _src->getGrayFrame();
    _mog2->apply(src,_fgMask);  // apply MOG2 (Gaussian Mixture-based Background/Foreground Segmentation Algorithm)
}

void Segmentation::RefineMask () {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    // Create a small cross-shaped structuring element

    cv::morphologyEx(_fgMask, _refinedMask, cv::MORPH_CLOSE, kernel);
    // cv::morphologyEx and cv::MORPH_CLOSE to fill small holes and connect
    // nearby foreground regions, making vehicles appear as solid blobs.

    cv::erode(_fgMask, _refinedMask, kernel);
    // Erode mask to remove small white noise points from the foreground mask
}

/*  Contour Detection Imlementation  */

ContourDetection::ContourDetection(std::shared_ptr<Segmentation> src) : _src(std::move(src)) {};

ContourDetection::ContourDetection(std::shared_ptr<Segmentation> src, double minArea)
: _src(std::move(src)), _minArea(minArea) {}

const std::vector<std::vector<cv::Point>>& ContourDetection::getContours() const { return _contours; }

const cv::Mat& ContourDetection::getDrawing() const { return _drawing; }

void ContourDetection::FindContours() {
    cv::Mat work = _src->getForegroundFrame().clone();
    _contours.clear();

    cv::findContours(work, _contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    // cv::findContours(image, contours, mode, method)


    _contours.erase(
        std::remove_if(_contours.begin(), _contours.end(),
                    [this](const std::vector<cv::Point>& c) {
                        return cv::contourArea(c) < _minArea; 
                    }),
        _contours.end()
    );

    _drawing = cv::Mat::zeros(work.size(), CV_8UC3);
    // Create blank image for drawing
    
    cv::drawContours(_drawing, _contours, -1, cv::Scalar(0, 255, 0), 2);
    // Draw all contours 
}

/*  Feature Extraction Imlementation  */

ContourFeatures::ContourFeatures(std::shared_ptr<ContourDetection> src) : _src(std::move(src)) {}

const std::vector<cv::Rect>& ContourFeatures::getBoundingBoxes() const { return _boxes; }
const std::vector<double>& ContourFeatures::getContourAreas() const { return _contourArea; }
const std::vector<cv::Point2d>& ContourFeatures::getCentroids() const { return _centroids; }
const std::vector<double>& ContourFeatures::getAspectRatio() const { return _aspectRatio; }
const std::vector<double>& ContourFeatures::getFillRatio() const { return _fillRatio; }

void ContourFeatures::ExtractFeatures() {
    _boxes.clear();
    _centroids.clear();
    _contourArea.clear();
    _aspectRatio.clear();

    for (const auto& contour : _src->getContours()) {
        if (contour.empty()) continue;
        cv::Rect box = cv::boundingRect(contour);
        // cv::boundingRect finds the smallest rectangle that completely encloses the input contour
        _boxes.push_back(box);

        // Calculate Centroids
        cv::Moments M = cv::moments(contour);
        cv::Point2d c;

        c.x = M.m10 / M.m00;
        c.y = M.m01 / M.m00;
        
        _centroids.push_back(c);

        // Calculate Areas
        double area = cv::contourArea(contour);
        _contourArea.push_back(area);

        // Calculate Aspect Ratios
        _aspectRatio.push_back(box.width/box.height);

        // Calculate Fill Ratios
        _fillRatio.push_back(area/(box.width*box.height));
    }
}