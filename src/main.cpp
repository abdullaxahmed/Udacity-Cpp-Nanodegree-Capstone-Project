#include "video_processing.h"
#include <iostream>

int main(){
    
    VideoReader vp("../videos/1.mp4");
    ColorConverter cc(vp);

    while(vp.readFrame()) {
        cc.ConvertFrameColor();
        cv::imshow("Original Preview", vp.getFrame());   // Original
        // cv::imshow("Grayscale Preview", cc.getGrayFrame());   // Grayscale
        cv::waitKey(25);    
    }

    return 0;
}