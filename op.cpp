#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;

int main() {
    const int width = 1200;
    const int height = 1024;
    const double theta_deg = 25.0;
    const double theta_rad = theta_deg * CV_PI / 180.0; // Convert degrees to radians

    Mat image(height, width, CV_8UC3, Scalar(255, 255, 255)); // Create a white image

    for (int r = 0; r < height; ++r) {
        for (int theta = 0; theta < width * 2 / 3; ++theta) { // Start from the middle of the left border and extend to two-thirds of the width
            double x = r * cos(theta * theta_rad);
            double y = r * sin(theta * theta_rad);
            Point point(static_cast<int>(x), static_cast<int>(height / 2 - y));
            if (point.x >= 0 && point.x < width * 2 / 3 && point.y >= 0 && point.y < height / 2) { // Limit to top-left two-thirds
                image.at<Vec3b>(point) = Vec3b(0, 0, 0); // Set the point to black
            }
        }
    }

    const int radius = 3; // Larger radius for points with r == 117

    for (int theta = 0; theta < width * 2 / 3; ++theta) { // Loop through the x-axis for two-thirds of the width
        double x = 117 * cos(theta * theta_rad); // Calculate x-coordinate for r = 117
        double y = 117 * sin(theta * theta_rad); // Calculate y-coordinate for r = 117
        Point center(static_cast<int>(x), static_cast<int>(height / 2 - y));
        if (center.x >= 0 && center.x < width * 2 / 3 && center.y >= 0 && center.y < height / 2) { // Limit to top-left two-thirds
            circle(image, center, radius, Scalar(255, 0, 0), -1); // Draw a filled circle with larger radius
        }
    }
    

    for (int theta = 0; theta < width * 2 / 3; ++theta) { // Loop through the x-axis for two-thirds of the width
        double x = 148 * cos(theta * theta_rad); // Calculate x-coordinate for r = 117
        double y = 148 * sin(theta * theta_rad); // Calculate y-coordinate for r = 117
        Point center(static_cast<int>(x), static_cast<int>(height / 2 - y));
        if (center.x >= 0 && center.x < width * 2 / 3 && center.y >= 0 && center.y < height / 2) { // Limit to top-left two-thirds
            circle(image, center, radius, Scalar(0, 255, 0), -1); // Draw a filled circle with larger radius
        }
    }
    
    for(int c=417; c<=440; ++c)
    {
    	 for (int theta = 0; theta < width * 2 / 3; ++theta) { // Loop through the x-axis for two-thirds of the width
        double x = c * cos(theta * theta_rad); // Calculate x-coordinate for r = 117
        double y = c * sin(theta * theta_rad); // Calculate y-coordinate for r = 117
        Point center(static_cast<int>(x), static_cast<int>(height / 2 - y));
        if (center.x >= 0 && center.x < width * 2 / 3 && center.y >= 0 && center.y < height / 2) { // Limit to top-left two-thirds
            circle(image, center, radius, Scalar(0, 0, 255), -1); // Draw a filled circle with larger radius
        }
    }	
    }

    imshow("Polar to Cartesian 2/3", image);
    waitKey(0);
    return 0;
}
