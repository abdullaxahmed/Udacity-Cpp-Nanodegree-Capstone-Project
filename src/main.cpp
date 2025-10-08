#include <iostream>

#include "video_processing.h"
#include "detector.h"
#include "display.h"
#include "config_io.h"

int main() {
    Params params = loadParams("../config/parameters.txt");

    auto videoReader = std::make_shared<VideoReader>(params.video);
    if (!videoReader->getCap().isOpened()) {
        std::cout << "Error: Cannot open video file!\n";
        return -1;  // exit error
    }

    auto colorConverter = std::make_shared<ColorConverter>(videoReader);
    auto segmentation = std::make_shared<Segmentation>(colorConverter, params.history, params.thresh, params.shadows);
    auto contourDetection = std::make_shared<ContourDetection>(segmentation, params.minarea);
    auto contourFeatures = std::make_shared<ContourFeatures>(contourDetection);


    Display display(*videoReader, *colorConverter, *segmentation, *contourDetection, *contourFeatures);
    Logger logger(params.log);

    processVideo(*videoReader, *colorConverter, *segmentation, *contourDetection, *contourFeatures, display, logger);

    return 0;
}