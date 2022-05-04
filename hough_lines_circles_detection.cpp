/**
 * @file hough_lines_circles_detection.cpp
 * @author Feras Alshehri (falshehri@mail.csuchico.edu)
 * @brief a program to detect lines, circles, or both. adapted with modifications from OpenCV's examples:
 *  1.
 * https://docs.opencv.org/4.1.1/d5/df9/samples_2cpp_2tutorial_code_2ImgTrans_2houghlines_8cpp-example.html
 *  2.
 * https://docs.opencv.org/4.1.1/d1/de6/samples_2cpp_2tutorial_code_2ImgTrans_2houghcircles_8cpp-example.html
 * @version 0.1
 * @date 2022-03-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// shared hough parameters
#define RADIUS_OF_DOT 1
#define DRAW_LINE_THICKNESS 3
#define BGR_BLUE (Scalar(255, 0, 0))
#define BGR_GREEN (Scalar(0, 255, 0))
#define BGR_RED (Scalar(0, 0, 255))
#define BGR_ORANGE (Scalar(0, 75, 255))
#define BGR_PURPLE (Scalar(255, 0, 135))
// specific defines
#define CANNY_THRESHOLD_MAX 50
#define CANNY_THRESHOLD_MIN 200
#define CANNY_APERTURE_SIZE 3
#define HOUGH_LINE_RHO 1
#define HOUGH_LINE_THETA 1
#define DEG_TO_RAD (CV_PI / 180)
#define HOUGH_LINE_THRESHOLD 150
#define HOUGH_LINE_SRN 0
#define HOUGH_LINE_STN 0
#define HOUGH_LINE_MIN_THETA 0
#define HOUGH_LINE_MAX_THETA CV_PI
#define HOUGH_LINE_P_THRESHOLD 50
#define HOUGH_LINE_P_MIN_LINE_LENGTH 50
#define HOUGH_LINE_P_MAX_LINE_GAP 10
#define HOUGH_LINE_DRAW_COLOR BGR_ORANGE
#define HOUGH_LINE_DRAW_THICKNESS DRAW_LINE_THICKNESS
// Hough circle defines
#define MEDIAN_BLUR_KSIZE 5
#define HOUGH_CIRCLE_DP 1 // Inverse ratio of accumulator resolution : image resolution
#define HOUGH_CIRCLE_MIN_DISTANCE_DEVIDER 1
#define HOUGH_CIRCLE_PARAM_1 100
#define HOUGH_CIRCLE_PARAM_2 30
#define HOUGH_CIRCLE_MIN_RADIUS 30
#define HOUGH_CIRCLE_MAX_RADIUS 100
#define HOUGH_CIRCLE_DRAW_DOT_COLOR BGR_RED
#define HOUGH_CIRCLE_DRAW_CIRCLE_COLOR BGR_GREEN
#define HOUGH_CIRCLE_DRAW_THICKNESS DRAW_LINE_THICKNESS
// GUI defines
#define WIN_TITLE "Hough line/circle detection"
#define SYSTEM_ERROR (-1)
#define ESCAPE_KEY (27)

using namespace cv;
using namespace std;

/**
 * @brief parse user input during runtime to change the enable/disable hough detection.
 *          Possible modes:
 *              - 'n': disable all hough detection.
 *              - 'l': enable hough lines detection.
 *              - 'c': enable hough circle detection.
 *              - 'b': enable both hough line and circle detection.
 *
 * @param winInput user input during runtime
 * @param mode current hough mode passed by refernce
 */
void parseUserInput(char winInput, char &mode) {
    if (mode != winInput) // ignore input if it matches current mode
    {
        if (winInput == 'n') {
            cout << "\t>> disable all hough detection requested" << endl;
            mode = 'n';
        } else if (winInput == 'l') {
            cout << "\t>> enabling hough line detection" << endl;
            mode = winInput;
        } else if (winInput == 'c') {
            cout << "\t>> enabling hough circle detection" << endl;
            mode = winInput;
        } else if (winInput == 'b') {
            cout << "\t>> enabling both hough line and circle detection" << endl;
            mode = 'b';
        } else {
            ; // no-op. ignore input.
        }
    } else {
        cout << "\t>> requested mode is currently enabled! (got [" << winInput;
        cout << "], current mode [" << mode << "])" << endl;
    }
}

/**
 * @brief apply and hough line detection on a single frame and overlay detected line.
 *
 * @param frame Mat object of the frame to apply hough line detection on, passed by
 * refernce and will be modified.
 * @param probabilistic boolean flag. Displays probabilistic frame rate
 */
static void doHoughLine(Mat &frame, bool probabilistic = false) {
    // declare variables and objects to hold our frame while processing temperorally
    Mat gray, dst, cdst;

    // convert to grey scale then apply a median blue to help reduce noise
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // Edge detection
    Canny(gray, dst, CANNY_THRESHOLD_MAX, CANNY_THRESHOLD_MIN, CANNY_APERTURE_SIZE);

    // // Copy edges to the images that will display the results in BGR
    // cvtColor(dst, cdst, COLOR_GRAY2BGR);

    if (!probabilistic) {
        vector<Vec2f> lines;

        // Standard Hough Line Transform
        HoughLines(dst, lines, HOUGH_LINE_RHO, HOUGH_LINE_THETA * DEG_TO_RAD,
                   HOUGH_LINE_THRESHOLD, HOUGH_LINE_SRN, HOUGH_LINE_STN,
                   HOUGH_LINE_MIN_THETA, HOUGH_LINE_MAX_THETA);

        // Draw the lines
        for (size_t i = 0; i < lines.size(); i++) {
            float  rho = lines[i][0], theta = lines[i][1];
            Point  pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * (a));
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * (a));
            line(frame, pt1, pt2, HOUGH_LINE_DRAW_COLOR, HOUGH_LINE_DRAW_THICKNESS,
                 LINE_AA);
        }
    } else {
        vector<Vec4i> lines;

        // Probabilistic Line Transform
        HoughLinesP(dst, lines, HOUGH_LINE_RHO, HOUGH_LINE_THETA * DEG_TO_RAD,
                    HOUGH_LINE_P_THRESHOLD, HOUGH_LINE_P_MIN_LINE_LENGTH,
                    HOUGH_LINE_P_MAX_LINE_GAP);
        // Draw the lines
        for (size_t i = 0; i < lines.size(); i++) {
            Vec4i l = lines[i];
            line(frame, Point(l[0], l[1]), Point(l[2], l[3]), HOUGH_LINE_DRAW_COLOR,
                 HOUGH_LINE_DRAW_THICKNESS, LINE_AA);
        }
    }

    return;
}

/**
 * @brief apply and hough circle detection on a single frame and overlay detected circle.
 *
 * @param frame Mat object of the frame to apply hough circle detection on, passed by
 * refernce and will be modified.
 */
static void doHoughCircle(Mat &frame) {
    // declare variables and objects to hold our frame while processing temperorally
    Mat           gray;
    vector<Vec3f> circles;

    // convert to grey scale then apply a median blue to help reduce noise
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, MEDIAN_BLUR_KSIZE);

    HoughCircles(gray, circles, HOUGH_GRADIENT, HOUGH_CIRCLE_DP,
                 gray.rows / HOUGH_CIRCLE_MIN_DISTANCE_DEVIDER, HOUGH_CIRCLE_PARAM_1,
                 HOUGH_CIRCLE_PARAM_2, HOUGH_CIRCLE_MIN_RADIUS, HOUGH_CIRCLE_MAX_RADIUS);

    // overlay detected circles on the original frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c      = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(frame, center, RADIUS_OF_DOT, HOUGH_CIRCLE_DRAW_DOT_COLOR,
               HOUGH_CIRCLE_DRAW_THICKNESS, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(frame, center, radius, HOUGH_CIRCLE_DRAW_CIRCLE_COLOR,
               HOUGH_CIRCLE_DRAW_THICKNESS, LINE_AA);
    }

    return;
}

/**
 * @brief main loop to run during runtime and control the state machine.
 *
 */
static void mainLoop() {
    char mode = 'n'; // initialize mode to no detection
    char winInput;

    // initialize video stream from device `cam0` and set resolution
    VideoCapture cam0(0);
    if (!cam0.isOpened()) { exit(SYSTEM_ERROR); }
    cam0.set(CAP_PROP_FRAME_WIDTH, 640);
    cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

    // create a window to display our video
    namedWindow(WIN_TITLE);

    while (1) {
        Mat frame; // Mat object to hold our frame

        // parse user input in during runtime
        if ((winInput = waitKey(10)) == ESCAPE_KEY) {
            break;
        } else {
            parseUserInput(winInput, mode);
        }

        // get frame from camera
        cam0.read(frame);

        // process frame based on mode
        switch (mode) {
        case 'n': // no hough detection
            ;     // no-op -- pass frame as-is
            break;

        case 'l': // hough line
            doHoughLine(frame);
            break;

        case 'c': // hough circle
            doHoughCircle(frame);
            break;

        case 'b': // both hough line and circle
            doHoughLine(frame);
            doHoughCircle(frame);
            break;

        default:
            break;
        }

        // display processed frame
        imshow(WIN_TITLE, frame);
    }

    cout << "\t>> Exiting!" << endl;
    destroyWindow(WIN_TITLE);

    return;
}

/**
 * @brief entry point of program.
 *
 * @param argc count of cmdl args.
 * @param argv list of strings, each representing a cmdl arg.
 * @return int returns 0 upon successful execution.
 */
int main(int argc, char **argv) {
    // define command line args
    CommandLineParser parser(argc, argv,
                             "{n         || disable any hough detection.}"
                             "{l         || enable hough line detection.}"
                             "{c         || enable hough circle detection.}"
                             "{b         || enable both hough line and circle detection.}"
                             "{ESC       || exit program.}"
                             "{help     h|false|show help message}");

    parser.printMessage();
    // @DEBUG
    cout << "v0.1" << endl;

    mainLoop();

    return 0;
}