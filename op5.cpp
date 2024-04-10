#include <iostream>
#include <opencv2/opencv.hpp>

// Function to generate a random B-scan data
std::vector<int> generateBscanData(int size) {
    std::vector<int> bscanData(size);
    for (int i = 0; i < size; ++i) {
    	if(i==0 || i==117 || i==255 || i==148 || (i>=417 && i<= 440)) {
    		bscanData[i] = 255;
    	}
    	else {
    		bscanData[i] = 0;
    	}
        // Randomly assign values of 0 or 255
        //bscanData[i] = rand() % 2 == 0 ? 0 : 255;
    }
    return bscanData;
}

// Function to generate an image from B-scan data
cv::Mat generateImageFromBscan(const std::vector<int>& bscanData, int width, int height) {
    cv::Mat image(height, width, CV_8UC1); // Grayscale image
    int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Assign pixel value from B-scan data
            image.at<uchar>(y, x) = bscanData[index++];
            if (index >= bscanData.size()) // Ensure we don't exceed B-scan data size
                break;
        }
        if (index >= bscanData.size())
            break;
    }
    return image;
}

int main() {
    const int width = 1024;  // Width of the image
    const int height = 400; // Height of the image
    const int numFrames = 100; // Number of frames in the video
    const int fps = 15; // Frames per second

    // Initialize OpenCV video writer
    cv::VideoWriter video("output_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height));

    // Generate random B-scan data for each frame and create video
    for (int i = 0; i < numFrames; ++i) {
        // Generate B-scan data
        std::vector<int> bscanData = generateBscanData(width * height);

        // Generate image from B-scan data
        cv::Mat frame = generateImageFromBscan(bscanData, width, height);

        // Write frame to video
        video.write(frame);
        
         // Display the frame (optional)
        cv::imshow("Frame", frame);
        cv::waitKey(1000 / fps); // Adjust delay to match desired frame rate
    }

    // Release the video writer
    video.release();

    return 0;
}

