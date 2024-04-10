#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    // B-scan data dimensions
    int width = 1024;
    int height = 400;
    int numFrames = 100; // Number of frames in the video
    int fps = 15; // Frames per second

    // Video writer object
    cv::VideoWriter videoWriter;
    videoWriter.open("bscan_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height), false);

    // Generate frames and write to video
    for (int frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        // Create a frame to store the B-scan data
        cv::Mat frame(height, width, CV_8UC1, cv::Scalar(0));

        // Simulate B-scan data (some rows have all white pixels, rest have all black pixels)
        if (frameIndex == 0 || frameIndex == 117 || frameIndex == 148 || frameIndex == 255 || (frameIndex >= 417 && frameIndex <= 440)) {
            frame.setTo(cv::Scalar(255)); // Set all pixels to white (255)
        }

        // Write the frame to the video
        videoWriter.write(frame);

        // Display the frame (optional)
        cv::imshow("Frame", frame);
        cv::waitKey(1000 / fps); // Adjust delay to match desired frame rate
    }

    // Release the video writer
    videoWriter.release();

    return 0;
}

