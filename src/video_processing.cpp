#include <thread>
#include <mutex>
#include <atomic>

#include "video_processing.h"
#include "display.h"
#include "detector.h"
#include "config_io.h" 


VideoReader::VideoReader(const std::string& path) : _path(path) {
    _cap.open(path);
}

VideoReader::~VideoReader() {
    if (_cap.isOpened()) {
        _cap.release();
    }
}

cv::VideoCapture& VideoReader::getCap() {
    return _cap;
}

void VideoReader::setCap(const std::string& path) {
    _cap.open(path);    // Open input video file
    _path = path;
}

const cv::Mat& VideoReader::getFrame() const {
    return _frame;
}

const std::string& VideoReader::getPath() const {
    return _path;
}

bool VideoReader::readFrame() {
    return _cap.read(_frame);
}

void VideoReader::setFrameForProcessing(const cv::Mat& f) {
    _frame = f;
}

ColorConverter::ColorConverter(std::shared_ptr<VideoReader> src) : _src(std::move(src)) {};

const cv::Mat& ColorConverter::getGrayFrame() const {
    return _grayFrame;
}

void ColorConverter::ConvertFrameColor() {
    const cv::Mat& src = _src->getFrame();
    cvtColor(src, _grayFrame, cv::COLOR_BGR2GRAY);
}

void ColorConverter::ConvertFrameColor(const cv::Mat& src) {
    cvtColor(src, _grayFrame, cv::COLOR_BGR2GRAY);  // Convert BGR frames to grayscale 
}


void processVideo(VideoReader& videoReader, ColorConverter& colorConverter, Segmentation& segmentation,
                  ContourDetection& contourDetection, ContourFeatures& contourFeatures,
                  Display& display, Logger& logger)
{
    FpsMeter fps;
    int frameIndex = 0;

    while (videoReader.readFrame()) {
        colorConverter.ConvertFrameColor();
        segmentation.RemoveBackground();
        segmentation.RefineMask();
        contourDetection.FindContours();
        contourFeatures.ExtractFeatures();
        display.renderBoxes();

        double f = fps.sample();
        logger.write(frameIndex, contourDetection.getCount(), f);
        frameIndex++;

        int key = display.show();
        if (key == 27) break; // ESC to exit
    }
}

std::future<double> processVideoConcurrent(VideoReader& videoReader, ColorConverter& colorConverter, Segmentation& segmentation,
                  ContourDetection& contourDetection, ContourFeatures& contourFeatures,
                  Display& display, Logger& logger)
{
    std::mutex m;
    cv::Mat sharedFrame;     // last captured frame
    bool newFrame = false;  
    std::atomic<bool> stop{false};

    // Promise/Future to return average FPS from worker
    std::promise<double> prom;
    auto fut = prom.get_future();

    /* Thread A: Reader */
    std::thread t_reader([&](){
        while (!stop.load(std::memory_order_relaxed)) {
            {
                std::lock_guard<std::mutex> lk(m);
                if (newFrame) {
                } else {
                    if (!videoReader.readFrame()) {
                        stop.store(true, std::memory_order_relaxed);
                        break;
                    }
                    sharedFrame = videoReader.getFrame().clone();
                    newFrame = true;
                    continue;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });


    /* Thread B: Worker */
    std::thread t_worker([&](){
        FpsMeter fps;
        using clock = std::chrono::steady_clock;
        auto t0 = clock::now();

        size_t processed = 0;
        int frameIndex = 0;

        while (true) {
            cv::Mat local;
            {
                std::lock_guard<std::mutex> lk(m);
                if (newFrame) {
                    local = sharedFrame;  // shallow copy
                    newFrame = false;
                }
            }

            if (local.empty()) {
                if (stop.load(std::memory_order_relaxed)) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            colorConverter.ConvertFrameColor(local);
            segmentation.RemoveBackground();
            segmentation.RefineMask();
            contourDetection.FindContours();
            contourFeatures.ExtractFeatures();

            display.renderBoxes(local);

            double f = fps.sample();
            logger.write(frameIndex, contourDetection.getCount(), f);
            ++frameIndex;
            ++processed;

            int key = display.show();
            if (key == 27) { // ESC to exit
                stop.store(true, std::memory_order_relaxed);
                break;
            }

        }

        auto t1 = clock::now();
        double secs = std::chrono::duration<double>(t1 - t0).count();
        double avg_fps = (secs > 0.0) ? (processed / secs) : 0.0;
        
        prom.set_value(avg_fps);
    });

    // Join and return
    t_reader.join();
    t_worker.join();
    return std::move(fut);
}