#include <iostream>

#include "video_processing.h"
#include "detector.h"

int main(){
    
    VideoReader vp("../videos/1.mp4");
    ColorConverter cc(vp);
    Segmentation fg(cc);
    ContourDetection c(fg);
    ContourFeatures f(c);

    while(vp.readFrame()) {
        cc.ConvertFrameColor();
        fg.RemoveBackground();
        fg.RefineMask();
        c.FindContours();
        f.ExtractFeatures();

        // cv::imshow("Original Preview", vp.getFrame());   // Original
        // cv::imshow("Grayscale Preview", cc.getGrayFrame());   // Grayscale
        // cv::imshow("Segmentation Preview", fg.getForegroundFrame());  // Background Removed
        // cv::imshow("Segmentation Refined Preview", fg.getRefinedFrame());  // Refined Foreground
        cv::imshow("Contours Preview", c.getDrawing());   // Contour Visualization 

        cv::Mat frame = vp.getFrame().clone();
        for (const auto& box : f.getBoundingBoxes()) {
            cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Bounding Boxes", frame);     

        cv::waitKey(25);    
    }

    return 0;
}