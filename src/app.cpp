#include <opencv2/opencv.hpp>
#include <iostream>

int main(){
    cv::VideoCapture cap("../videos/1.mp4"); // Open input video file
    if(!cap.isOpened()) {
        std::cout << "Error: failed to read video file." << std::endl;
    }
    else {
        std::cout << "Video file read successfully!" << std::endl;
    }

    cv::Mat frame; // Matrix object to hold a single video frame
    while(true){
        if(!cap.read(frame)) break; // Read the next frame from the video; break if no more frames
        cv::imshow("Preview",frame);
        cv::waitKey(25);
    }
    return 0;
}