#include <iostream>

#include "video_processing.h"
#include "detector.h"
#include "display.h"

int main(){
    
    VideoReader videoReader("../videos/highway.mp4");
    ColorConverter colorConverter(videoReader);
    Segmentation segmentation(colorConverter);
    ContourDetection contourDetection(segmentation);
    ContourFeatures contourFeatures(contourDetection);
    Display display(videoReader, colorConverter, segmentation, contourDetection, contourFeatures);

while (videoReader.readFrame()) {
    colorConverter.ConvertFrameColor();
    segmentation.RemoveBackground();
    segmentation.RefineMask();
    contourDetection.FindContours();
    contourFeatures.ExtractFeatures();

    display.renderBoxes();
    display.show();

    // // cv::imshow("Gray", colorConverter.getGrayFrame());
    // // cv::imshow("ForegroundMask", segmentation.getForegroundFrame());
    // // cv::imshow("RefinedMask", segmentation.getRefinedFrame());
    // // cv::imshow("Contours", contourDetection.getDrawing());
}

    return 0;
}