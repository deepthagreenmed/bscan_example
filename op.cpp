#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cmath>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

Display* dpy;
Window win;
GLXContext ctx;
int width = 1024;
int height = 400;
int numFrames = 0;

void createWindow() {
    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Error: Could not open X display\n");
        exit(1);
    }

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    static int visual_attribs[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };

    XVisualInfo *vi = glXChooseVisual(dpy, screen, visual_attribs);
    if (!vi) {
        fprintf(stderr, "Error: No appropriate visual found\n");
        exit(1);
    }

    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    if (!win) {
        fprintf(stderr, "Error: Failed to create window\n");
        exit(1);
    }

    ctx = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    if (!ctx) {
        fprintf(stderr, "Error: Failed to create OpenGL context\n");
        exit(1);
    }

    glXMakeCurrent(dpy, win, ctx);

    XMapWindow(dpy, win);
}

void destroyWindow() {
    glXMakeCurrent(dpy, None, nullptr);
    glXDestroyContext(dpy, ctx);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void drawFrame() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    double angle = 22.5; // Angle in degrees
    double angle_rad = angle * 3.14159 / 180.0;
    double yOffset = 200;

    // Draw points in Cartesian coordinates
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (double i = -angle_rad; i < angle_rad; i += angle_rad / 200) {
        for (int j = 0; j < width; ++j) {
            double x = j * cos(i);
            double y = j * sin(i) + yOffset; // Add yOffset

            // Set white color for specific points
            if (j == 117 || j == 148 || (j >= 417 && j <= 440)) {
                glColor3f(1.0f, 1.0f, 1.0f); // white
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // black
            }

            // Convert to OpenGL coordinates
            double gl_x = 2.0 * x / width - 1.0;
            double gl_y = 2.0 * y / height - 1.0;

            glVertex2d(gl_x, gl_y);
        }
    }
    glEnd();

    // Draw frame number
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    glRasterPos2f(-1.0f, -0.9f); // Place the text in the lower-left corner
    std::string frameStr = "Frame number: " + std::to_string(numFrames);
    for (char c : frameStr) {
        //glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glXSwapBuffers(dpy, win);
}


void renderLoop() {
    constexpr std::chrono::milliseconds targetFrameDuration(66); // 1000ms / 60FPS ≈ 16.67ms per frame
    std::chrono::steady_clock::time_point frameStartTime;
    std::chrono::steady_clock::time_point frameEndTime;

    while (true) {
        frameStartTime = std::chrono::steady_clock::now();

        XEvent event;
        while (XPending(dpy) > 0) {
            XNextEvent(dpy, &event);
            switch (event.type) {
                case Expose:
                    drawFrame();
                    break;
                case KeyPress:
                    destroyWindow();
                    return;
            }
        }

        drawFrame(); // Draw frame outside event handling

        frameEndTime = std::chrono::steady_clock::now();
        auto frameElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);

        auto sleepTime = targetFrameDuration - frameElapsedTime;
        if (sleepTime > std::chrono::milliseconds::zero()) {
            std::this_thread::sleep_for(sleepTime);
        } else {
            // If rendering took longer than expected, print a warning
            std::cerr << "Warning: Rendering took longer than expected." << std::endl;
        }
    }
}



int main() {
    createWindow();
    renderLoop();
    return 0;
}
