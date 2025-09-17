#include "video_processing.h"
#include "detector.h"
#include <iostream>

int main(){
    
    VideoReader vp("../videos/1.mp4");
    ColorConverter cc(vp);
    Segmentation fg(cc);

    while(vp.readFrame()) {
        cc.ConvertFrameColor();
        fg.RemoveBackground();
        fg.RefineMask();
        // cv::imshow("Original Preview", vp.getFrame());   // Original
        // cv::imshow("Grayscale Preview", cc.getGrayFrame());   // Grayscale
        // cv::imshow("Segmentation Preview", fg.getForegroundFrame());  // Background Removed
        cv::imshow("Segmentation Refined Preview", fg.getRefinedFrame()); 
        cv::waitKey(25);    
    }

    return 0;
}