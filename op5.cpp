#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    // Video dimensions
    int width = 400;
    int height = 400;
    int numFrames = 100; // Number of frames in the video
    int fps = 15; // Frames per second

    // Video writer object
    cv::VideoWriter videoWriter;
    videoWriter.open("binary_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height));

    // Generate frames and write to video
    for (int frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        // Create a frame
        cv::Mat frame(height, width, CV_8UC1, cv::Scalar(0)); // Single channel for binary image

        // Set some pixels to 255 (white) and others to 0 (black)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Set every alternate pixel to white
                if (((x + y + frameIndex) == 0) || ((x + y + frameIndex) == 117) || ((x + y + frameIndex) == 255) || ((x + y + frameIndex) == 148) || ((x + y + frameIndex) >= 417 && ((x + y + frameIndex) <= 440))) {
                    frame.at<uchar>(y, x) = 255; // Set pixel to white
                }
            }
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
