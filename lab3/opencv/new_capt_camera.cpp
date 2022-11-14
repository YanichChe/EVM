#include <iostream>            // for standard I/O
#include <string>              // for strings
#include <iomanip>             // for controlling float print precision
#include <sstream>             // string to number conversion
#include <opencv2/core.hpp>    // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp> // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> // OpenCV window I/O
#include <opencv2/imgcodecs.hpp>
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    VideoCapture cap(0); // captRefrnc(sourceReference);//, captUndTst(sourceCompareWith);

    Mat img;
    Mat newImage;
    int frameCounter = 0;
    double totalTime = 0, inputTime = 0, procTime = 0, outputTime = 0;

    while (true)
    {
        clock_t c_start = clock();
        cap.read(img);
        inputTime += (clock() - c_start);

        c_start = clock();
        cvtColor(img, newImage, COLOR_BGR2HSV);
        // Canny(img, newImage1, 100, 200)
        procTime += (clock() - c_start);

        c_start = clock();
        imshow("Modificed Image", newImage);
        outputTime += (clock() - c_start);

        frameCounter++;

        char c = waitKey(10);
        if (c == 27)
            break;
    }

    totalTime = clock();
    double percent = totalTime / 100.1;

    cout << "Frame rate: " << (frameCounter / (totalTime / CLOCKS_PER_SEC)) << endl;
    cout << "Input time: " << inputTime / percent << "%" << endl;
    cout << "Processing time: " << procTime / percent << "%" << endl;
    cout << "Output time: " << outputTime / percent << "%" << endl;

    return 0;
}
