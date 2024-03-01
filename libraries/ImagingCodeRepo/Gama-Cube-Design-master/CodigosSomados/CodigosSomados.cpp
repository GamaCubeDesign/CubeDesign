#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

//Achar os centroides
struct centroides { // Struct que guardara os centroids
    vector<Point2f> centroids; //Vetor que guarda todos os centroids encontrados
    vector<int> visited; //Vetor que guarda se o centroid foi visitado ou nao
};

int main() {

    // Recebendo entrada de qual video sera tratado
    int numVideo;
    cout << "Escolha um video: (1 ou 2 ou 3)\n";
    cin >> numVideo;
    string stringVideo = to_string(numVideo);
    // Recebendo entrada de qual tratamento sera realizado
    int scene;
    cout << "Escolha uma cena: (1 ou 2)\n";
    cin >> scene;


    // Definindo o video a ser tratado de acordo com a entrada do usuario
    string videoPath;
    if (numVideo == 1) {
        videoPath = "gif0" + stringVideo + ".gif";
    }
    else if (numVideo == 2){
        videoPath = "gif0" + stringVideo + ".mp4";
    }
    else {
        videoPath = "gif0" + stringVideo + ".mp4";
    }
    // Criar VideoCapture
    VideoCapture capture(videoPath);

    //Checando se abriu o video
    if (!capture.isOpened()) {
        printf("Erro.");
        return -1;
    }

    int numberOfFrames = 0;
    // Loop para pegar cada frame
    while (true) {
        // Ler o frame
        Mat frame;
        capture >> frame;

        // Fechar o loop se tiver acabado o video
        if (frame.empty()) {
            break;
        }
        string filename = "frames_gif_0" + stringVideo + "/frame_" + to_string(numberOfFrames++) + ".png";
        imwrite(filename, frame);
    }

    //Matriz de visitados
    vector<vector<short int>> matrizVisitados(numberOfFrames);

    for (size_t k = 0; k < numberOfFrames-1; k++)
    {
        int indexCurrentFrame = k;
        if (scene == 1) {
            //Calcular centroide desse raio para realizar pointPolygonTest
            //Realizando tratamento da imagem pra achar novamente o contorno
            Mat currentFrame = imread("frames_gif_0" + stringVideo + "/frame_" + to_string(indexCurrentFrame) + ".png", 0);
            GaussianBlur(currentFrame, currentFrame, Size(5, 5), 0);
            threshold(currentFrame, currentFrame, 200, 255, THRESH_BINARY);
            vector<Vec4i> hierarchy;
            vector<vector<Point>> contours;

            Mat output = Mat::zeros(currentFrame.size(), CV_8UC3);
            cv::findContours(currentFrame, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE); //Contornos encontrados

            //Discriminar centroides
            vector<Point2f> finalCentroids(contours.size()); //Vetor que guarda os centroids apos discriminacao
            struct centroides c; // Inicializacao da struct
            c.centroids.resize(contours.size()); // Inicializacao do vetor centroids com o tamanho do numero de contours q existem
            c.visited.resize(contours.size(), 0); // Inicializando com valor 0 todos os membros do vetor visited
            //Loop para pegar todos os centroides
            for (size_t i = 0; i < contours.size(); ++i) {
                // Calcular os moments
                Moments mu = moments(contours[i]);
                // Calcular coordenadas dos centroides
                c.centroids[i].x = mu.m10 / mu.m00;
                c.centroids[i].y = mu.m01 / mu.m00;
            }

            //Loop que percorre por todos os centroides e salva depois no vetor finalcentroids
            for (size_t i = 0; i < c.centroids.size(); i++) {
                int biggestPoint = i;
                if (!c.visited[i]) { // Se o centroid nao tiver sido visitado ainda, compare ele com os demais
                    for (size_t j = i + 1; j < c.centroids.size(); j++) {
                        if (c.visited[j]) continue; // Se o centroid ja foi visitado, pula pro proximo
                        std::cout << abs(c.centroids[i].x - c.centroids[j].x) << "  " << abs(c.centroids[i].y - c.centroids[j].y) << endl;
                        if (abs(c.centroids[i].x - c.centroids[j].x) <= 30 && abs(c.centroids[i].y - c.centroids[j].y) <= 30) { // abs serve para achar o modulo, se o modulo da diferenca for menor/igual a 20 entra
                            if (contours[biggestPoint].size() < contours[j].size()) biggestPoint = j;
                            c.visited[j] = 1;
                        }
                    }
                    finalCentroids[i] = c.centroids[biggestPoint];
                }
                c.visited[i] = 1;
            }

            matrizVisitados[k].resize(finalCentroids.size(), 0);

            int duration = 1;
            for (size_t j = 0; j < finalCentroids.size(); j++) {
                int nextIndex = indexCurrentFrame+1+j;
                //Agora achar duracao de cada centroide do frame atual
                Mat nextFrame = imread("camera_video\\frame_" + to_string(nextIndex) + ".png", 0);
                GaussianBlur(nextFrame, nextFrame, Size(5, 5), 0);
                threshold(nextFrame, nextFrame, 200, 255, THRESH_BINARY);
                vector<Vec4i> nextHierarchy;
                vector<vector<Point>> nextContours;
                cv::findContours(nextFrame, nextContours, nextHierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE); //Contornos encontrados
                //Se o valor do beforeCheckDuration nao mudar, e pq nenhum raio passou no pointPolygonTest
                int beforeCheckDuration = duration;
                Point2f centroide = finalCentroids[j];
                for (size_t i = 0; i < nextContours.size(); i++)
                {
                    int isInside = pointPolygonTest(nextContours[i], centroide, true);
                    if (isInside >= -10) {
                        //Se estiver dentro de 10px de distancia do centroide atual, atualizar o centroide
                        // Calcular os moments
                        Moments mu = moments(nextContours[i]);
                        // Calcular coordenadas dos centroides
                        centroide.x = mu.m10 / mu.m00;
                        centroide.y = mu.m01 / mu.m00;
                        duration++;
                        std::cout << isInside << " e frame numero: " << nextIndex << " e centroide: " << centroide << endl;
                        break;
                    }
                }
                // Quando chegar no frame pra frente que o raio acaba, inverte o count pra comecar a procurar quando ele comeca
                if (beforeCheckDuration == duration) {
                    //Calculo do centroide do Rocha
                    //Salvamento da duracao e do valor do centroide
                    continue;
                }
            }

        }

    }
    
    waitKey(0);

    return 0;
}