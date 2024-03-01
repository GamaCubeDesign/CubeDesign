#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {

    // Recebendo entrada de qual vídeo sera tratado
    int numVideo;
    std::cout << "Escolha um video: (1 ou 2)\n";
    std::cin >> numVideo;
    string stringVideo = to_string(numVideo);

    // Definindo o video a ser tratado de acordo com a entrada do usuario
    string videoPath;
    if (numVideo == 1) {
        videoPath = "C:\\Users\\Lucas Bergholz\\Documents\\GAMA CUBE DESIGN\\OPENCV-TESTE\\gif0" + stringVideo + ".gif";
    }
    else {
        videoPath = "C:\\Users\\Lucas Bergholz\\Documents\\GAMA CUBE DESIGN\\OPENCV-TESTE\\gif0" + stringVideo + ".mp4";
    }
    // Criar VideoCapture
    cv::VideoCapture capture(videoPath);

    //Checando se abriu o video
    if (!capture.isOpened()) {
        printf("Erro.");
        return -1; 
    }

    int count = 1;
    // Loop para pegar cada frame
    while (true) {
        // Ler o frame
        cv::Mat frame;
        capture >> frame;

        // Fechar o loop se tiver acabado o video
        if (frame.empty()) {
            break;
        }

        string filename = "frames_gif_0" + stringVideo + "/frame_" + to_string(count++) + ".png";
        cv::imwrite(filename, frame);

        //frame.at<cv::Vec3b>() = 


        cv::imshow("GIF Frame", frame);
        cv::waitKey(10);
    }

    cv::Mat framezin = imread("frames_gif_01\\frame_1.png");
    cv::imshow("framezin", framezin);

    waitKey(0);

    return 0;
}