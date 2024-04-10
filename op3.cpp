#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

// Function to generate synthetic B-scan data for a single frame with a moving object
void generateBScan(cv::Mat& frame, int frameIndex) {
    // Dimensions of the frame
    int width = frame.cols;
    int height = frame.rows;

    // Define parameters for the synthetic B-scan
    float centerAngle = M_PI / 2; // Center angle (in radians)
    float maxRadius = std::min(width, height) / 2; // Max radius
    float angleIncrement = M_PI / width; // Angle increment
    float radiusIncrement = maxRadius / height; // Radius increment

    // Position of the moving object (circle)
    float centerX = width / 2 + cos(frameIndex * 0.05) * width / 4; // Move in x-direction
    float centerY = height / 2 + sin(frameIndex * 0.05) * height / 4; // Move in y-direction
    float objectRadius = 20; // Radius of the moving object

    // Generate B-scan data for this frame with the moving object
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Calculate polar coordinates
            float angle = centerAngle + (x - width / 2) * angleIncrement;
            float radius = y * radiusIncrement;

            // Check if the current point is inside the moving object
            float distanceToCenter = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
            if (distanceToCenter < objectRadius) {
                // Inside the object, set intensity value to maximum
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
            } else {
                // Outside the object, set intensity value based on distance
                int intensity = 255 - static_cast<int>(distanceToCenter / objectRadius * 255);
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(intensity, intensity, intensity);
            }
        }
    }
}

int main() {
    // Video dimensions
    int width = 400;
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

        // Generate B-scan data for this frame with a moving object
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

