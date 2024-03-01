#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat frameCinza = imread("frames_gif_01\\frame_49.png", 0);

    Mat frameThresh;

    threshold(frameCinza, frameThresh, 200, 255, THRESH_BINARY);
    imshow("Cinza", frameCinza);
    //imshow("Threshold", frameThresh);

    vector<Vec4i> hierarchy;
    vector<vector<Point>> contours;

    Mat output = Mat::zeros(frameCinza.size(), CV_8UC3);

    findContours(frameThresh, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);


    int count = 0;
    int max = 0;
    int index = 0;
    for (auto i : contours){
        int pass = contours[count].size();
        if (pass >= max) { max = pass; index = count; }
        cout << contours[count++] << endl;
    }
    drawContours(output, contours, index, Scalar(0, 255, 0), 3);
    cout << index << endl;
    imshow("Oie", output);

    waitKey(0);

    return 0;
}