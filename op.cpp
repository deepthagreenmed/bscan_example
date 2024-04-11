#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

// Function to generate B-scan data for a single frame with set values at certain points
void generateBScan(cv::Mat& frame, int timeStep) {
    // Dimensions of the frame
    int width = frame.cols;
    int height = frame.rows;

    // Define parameters for the synthetic B-scan
    double maxDepth = height; // Max depth (along the scan line)
    double maxIntensity = 255.0; // Max intensity

    // Generate B-scan data for this frame
    for (int y = 0; y < height; ++y) {
        // Calculate intensity value based on depth
        double intensity = maxIntensity * (1 - static_cast<double>(y) / maxDepth);

        // Set specific intensity values at certain points based on x-coordinate
        if (y == height / 2) {
            for (int x = 0; x < width; ++x) {
                if (x == 0 || x == 117 || x == 148 || (x >= 417 && x <= 440)) {
                    // Convert polar coordinates to Cartesian coordinates
                    double radius = y;
                    double angle = static_cast<double>(x) / width * 2 * CV_PI;
                    int cartesianX = static_cast<int>(radius * cos(angle) + width / 2);
                    int cartesianY = static_cast<int>(radius * sin(angle) + height / 2);

                    // Ensure Cartesian coordinates are within bounds
                    cartesianX = std::max(0, std::min(cartesianX, width - 1));
                    cartesianY = std::max(0, std::min(cartesianY, height - 1));

                    // Set intensity value to white at Cartesian coordinates
                    frame.at<cv::Vec3b>(cartesianY, cartesianX) = cv::Vec3b(255, 255, 255);
                }
            }
        }
    }

    // Add a timestamp to the frame
    //cv::putText(frame, "Time Step: " + std::to_string(timeStep), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
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

        // Generate B-scan data for this frame
        generateBScan(frame, frameIndex);

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
