#include <iostream>

#include "video_processing.h"
#include "detector.h"
#include "display.h"
#include "config_io.h"

int main() {
    Params params = loadParams("../config/parameters.txt");

    VideoReader videoReader(params.video);
    if (!videoReader.getCap().isOpened()) {
        std::cout << "Error: Cannot open video file!\n";
        return -1;
    }

    ColorConverter colorConverter(videoReader);
    Segmentation segmentation(colorConverter, params.history, params.thresh, params.shadows);
    ContourDetection contourDetection(segmentation, params.minarea);
    ContourFeatures contourFeatures(contourDetection);
    Display display(videoReader, colorConverter, segmentation, contourDetection, contourFeatures);
    Logger logger(params.log);

    processVideo(videoReader, colorConverter, segmentation, contourDetection, contourFeatures, display, logger);
    return 0;
}