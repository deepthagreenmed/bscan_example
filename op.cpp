#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>   // For std::chrono
#include <thread>   // For std::this_thread::sleep_for

// Global variables
int width = 1024;
int height = 400;
double angle = 22.5; // Angle in degrees
double angle_rad;

// Offset to move the output down
int yOffset = 200;

// Video writer object
//cv::VideoWriter videoWriter;

void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // Set up orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawFrame(int frameNumber) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate angle for this frame
    double frameAngle = angle * static_cast<double>(frameNumber) / 400.0;
    double frameAngle_rad = frameAngle * M_PI / 180.0;

    // Draw points in Cartesian coordinates based on polar coordinates
    for (double i = -frameAngle_rad; i < frameAngle_rad; i += frameAngle_rad / 200) {
        for (int j = 0; j < width; ++j) {
            double x = j * cos(i);
            double y = j * sin(i) + yOffset; // Add yOffset

            if (j == 117 || j == 148 || (j >= 417 && j <= 440)) {
                glColor3f(1.0f, 1.0f, 1.0f); // White
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            }

            glBegin(GL_POINTS);
            glVertex2i(static_cast<int>(x), static_cast<int>(y));
            glEnd();
        }
    }

    glutSwapBuffers();
    glFlush();

    // Capture the frame and save it as an image
    /*std::stringstream filename;
    filename << "frame_" << std::setw(4) << std::setfill('0') << frameNumber << ".png";

    // Read pixels from the framebuffer
    unsigned char* pixels = new unsigned char[3 * width * height];
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    // Create OpenCV Mat from the rendered frame
    cv::Mat frame(height, width, CV_8UC3, pixels);

    // Write the frame to the video
    videoWriter.write(frame);

    delete[] pixels;*/
}

void captureFrames() {
    /*// OpenCV VideoWriter parameters
    std::string filename = "output2.avi";
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double fps = 60.0;
    cv::Size frameSize(width, height);

    // Initialize VideoWriter
    videoWriter.open(filename, codec, fps, frameSize);*/

    // Draw and capture each frame
    for (int i = 0; i < 400; ++i) {
        drawFrame(i);
        glutPostRedisplay(); // Trigger a redraw for the next frame
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / 60)); // Adjust this delay according to desired frame rate
    }

    // Release VideoWriter
    //videoWriter.release();
}

int main(int argc, char** argv) {
    angle_rad = angle * M_PI / 180.0;

    // Initialize OpenGL and GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Cartesian B-scan Video");

    initGL();

    // Capture frames and save as video
    captureFrames();

    return 0;
}
