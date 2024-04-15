#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

// Function to convert polar coordinates to Cartesian coordinates
Point polarToCartesian(double r, double theta) {
    double x = r * cos(theta);
    double y = r * sin(theta);
    return Point(x, y);
}

int main() {
    // Assuming your B-scan data is represented as a polar matrix
    // Replace this with your actual data loading code

    int rows = 1024; // Assuming your data has 500 rows
    int cols = 400; // Assuming each degree is represented

    // Create a blank image to draw the Cartesian representation
    Mat cartesianImage(rows, cols, CV_8UC1, Scalar(255)); // Initialize with white

    if (cartesianImage.empty()) {
        cerr << "Failed to create the Cartesian image." << endl;
        return 1;
    }

    // Iterate over the polar matrix
    for (int r = 0; r < rows; ++r) {
        for (int theta = 0; theta < cols; ++theta) {
            // Check conditions for r values
            if (r == 117 || r == 148 || (r >= 417 && r <= 440)) {
                // Convert polar coordinates to Cartesian
                Point currentPoint = polarToCartesian(r, theta * CV_PI / 180.0); // Convert theta to radians
                
                // Draw a line connecting the current point to the previous point
                if (theta > 0) {
                    Point previousPoint = polarToCartesian(r, (theta - 1) * CV_PI / 180.0);
                    line(cartesianImage, previousPoint, currentPoint, Scalar(0), 1, LINE_AA); // Draw in black
                }
            }
        }
    }

    // Display the Cartesian image
    imshow("Cartesian Image", cartesianImage);
    waitKey(0);
    return 0;
}
