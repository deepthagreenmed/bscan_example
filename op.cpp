#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <cstdio>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>

// Global variables
int width = 1024;
int height = 400;
double angle = 22.5; // Angle in degrees
double angle_rad;

// Offset to move the output down
int yOffset = 200;

// Video writer object
cv::VideoWriter videoWriter;

// Global variable
std::vector<std::vector<cv::Point>> frameBuffer;

// Global variables for timing
std::chrono::steady_clock::time_point startTime;
std::chrono::steady_clock::time_point endTime;

void startTimer() {
    startTime = std::chrono::steady_clock::now();
}

void stopTimer() {
    endTime = std::chrono::steady_clock::now();
}

void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // Set up orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawFrame(int frameNumber, std::vector<std::pair<int, int>>& pointBuffer) {
    // Calculate angle for this frame
    double frameAngle = angle * static_cast<double>(frameNumber) / 400.0;
    double frameAngle_rad = frameAngle * M_PI / 180.0;

    // Clear the buffer
    pointBuffer.clear();

    // Populate the buffer with points
    for (double i = -frameAngle_rad; i < frameAngle_rad; i += frameAngle_rad / 200) {
        for (int j = 0; j < width; ++j) {
            double x = j * cos(i);
            double y = j * sin(i) + yOffset; // Add yOffset

            // Save points to buffer
            pointBuffer.push_back(std::make_pair(static_cast<int>(x), static_cast<int>(y)));
        }
    }
}

void drawPoints(const std::vector<std::pair<int, int>>& pointBuffer) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw points using the data in the buffer
    for (const auto& point : pointBuffer) {
        int x = point.first;
        int y = point.second;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            if (x == 117 || x == 148 || (x >= 417 && x <= 440)) {
                glColor3f(1.0f, 1.0f, 1.0f); // White
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            }

            glBegin(GL_POINTS);
            glVertex2i(x, y);
            glEnd();
        }
    }

    glutSwapBuffers();
}

void captureFrames() {
    std::string filename = "output2.avi";
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double desiredFps = 15.0; // Desired frame rate
    cv::Size frameSize(width, height);

    videoWriter.open(filename, codec, desiredFps, frameSize);

    startTimer(); // Start the timer

    // Calculate the target duration for each frame to achieve the desired frame rate
    std::chrono::microseconds targetFrameDuration(static_cast<long long>(1000000 / desiredFps));

    std::vector<std::pair<int, int>> pointBuffer; // Buffer to store x and y points

    for (int i = 0; i < 400; ++i) {
        auto frameStartTime = std::chrono::steady_clock::now(); // Start time of the frame rendering

        drawFrame(i, pointBuffer);
        drawPoints(pointBuffer);
        glutPostRedisplay(); // Trigger a redraw for the next frame

        auto frameEndTime = std::chrono::steady_clock::now(); // End time of the frame rendering
        auto frameElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEndTime - frameStartTime);

        // Adjust the sleep duration based on frame rendering time to match the desired frame rate
        std::chrono::microseconds remainingTime = targetFrameDuration - frameElapsedTime;
        if (remainingTime.count() > 0) {
            std::this_thread::sleep_for(remainingTime);
        }
    }

    stopTimer(); // Stop the timer

    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    double seconds = duration.count() / 1000000.0; // Convert microseconds to seconds
    std::cout << "Video duration: " << seconds << " seconds" << std::endl;

    videoWriter.release();
}


int main(int argc, char** argv) {
    angle_rad = angle * M_PI / 180.0;

    // Initialize OpenGL and GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL Cartesian B-scan");

    initGL();

    // Capture frames and save as video
    captureFrames();

    return 0;
}
