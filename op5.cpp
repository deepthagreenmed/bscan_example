#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

// Function to generate B-scan data for a single frame with set values at certain points
void generateBScan(cv::Mat& frame) {
    // Dimensions of the frame
    int width = frame.cols;
    int height = frame.rows;

    // Define parameters for the synthetic B-scan
    float centerAngle = M_PI / 2; // Center angle (in radians)
    float maxRadius = std::min(width, height) / 2; // Max radius
    float angleIncrement = M_PI / width; // Angle increment
    float radiusIncrement = maxRadius / height; // Radius increment

    // Generate B-scan data for this frame without the moving object
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Calculate polar coordinates
            float angle = centerAngle + (x - width / 2) * angleIncrement;
            float radius = y * radiusIncrement;

            // Convert polar coordinates to Cartesian coordinates
            int cartX = width / 2 + radius * cos(angle);
            int cartY = height - (height / 2 + radius * sin(angle));

            // Set intensity value to zero
            // Check if the current point is within the specified region
            if (x == 0 || x == 117 || x == 148 || (x >= 417 && x <= 440)) {
                // Set intensity value to maximum
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
            } else {
                // Set intensity value to zero
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            //frame.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }
}

int main() {
    // Video dimensions
    int width = 1024;
    int height = 400;
    int numFrames = 100; // Number of frames in the video
    int fps = 15; // Frames per second

    // Video writer object
    cv::VideoWriter videoWriter;
    videoWriter.open("bscan_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height));

    // Generate frames and write to video
    for (int frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        // Create a frame to store the B-scan data
        cv::Mat frame(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

        // Generate B-scan data for this frame without the moving object
        generateBScan(frame);

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
