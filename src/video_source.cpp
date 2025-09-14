#include "video_source.h"
#include <iostream>

bool videoProcess::readFrame() { return _cap.read(_frame);}

int main(){
    
    videoProcess vp("../videos/1.mp4");

    while(vp.readFrame()) {
        cv::imshow("Preview", vp.getFrame());
        cv::waitKey(25);
    }

    return 0;
}