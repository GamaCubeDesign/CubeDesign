#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <thread>

using namespace std;
using namespace cv;

// Variavel de distancia dos centroides de parametro
int distance = 60;

// Variável para indicar se o loop deve ser interrompido
bool stopLoop = false;

// Função que verifica a entrada do teclado
void checkInput() {
    while (true) {
        if (cin.get() == 'q') {
            ::stopLoop = true;
            break;
        }
    }
}

double twoPointDistance(Point2f A, Point2f B) {
    double result = sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y) * (A.y - B.y));
    return result;
}


int main() {
    // Recebendo entrada de qual tratamento sera realizado
    int scene;
    std::cout << "Escolha uma cena: (1 ou 2)\n";
    cin >> scene;

    // Criar VideoCapture
    VideoCapture capture(0);

    //Checando se abriu o video
    if (!capture.isOpened()) {
        printf("Erro no VideoCapture.");
        return -1;
    }

    int numberOfFrames = 0;
    thread inputThread(checkInput);
    // Loop para pegar cada frame
    while (!::stopLoop) {
        // Ler o frame
        Mat frame;
        capture >> frame;
        // Grava o frame no folder designado
        imwrite("frame_" + to_string(numberOfFrames++) + ".png", frame);
        
        // Escolhe o tempo de distancia entre os frames
        // Adiciona um pequeno delay
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    inputThread.join();

    // Vetores que guardarao as maiores areas por frame e quais centroides ja foram visitados
    // O indice das 3 matrizes coincide para o mesmo raio
    vector<vector<double>> biggestAreas(numberOfFrames); //Matriz que vai guardar por frame a area de todos os raios
    vector<vector<short int>> matrixVisited(numberOfFrames); // Matriz de visitados para resolver cena 1
    vector<vector<Point2f>> allCentroids(numberOfFrames); // Matriz que guarda os centroides de todos os raios achados, apos discriminacao

    int countOfFrames = 1;
    numberOfFrames--;
    while (numberOfFrames--) {
        Mat greyFrame = imread("frame_" + to_string(countOfFrames++) + ".png", 0);
        GaussianBlur(greyFrame, greyFrame, Size(5, 5), 0);
        Mat threshFrame;

        threshold(greyFrame, threshFrame, 200, 255, THRESH_BINARY);

        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours;
        cv::findContours(threshFrame, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);        

        //Achar os centroides
        struct centroides { // Struct que guardara os centroids
            vector<Point2f> centroids; //Vetor que guarda todos os centroids encontrados
            vector<int> visited; //Vetor que guarda se o centroid foi visitado ou nao
        };

        struct centroides c; // Inicializacao da struct
        c.centroids.resize(contours.size()); // Inicializacao do vetor centroids com o tamanho do numero de contours q existem
        c.visited.resize(contours.size(), 0); // Inicializando com valor 0 todos os membros do vetor visited

        // Print do frame
        printf("\n%d:\n\n", countOfFrames - 1);

        //Loop para pegar todos os centroides
        for (size_t i = 0; i < contours.size(); ++i) {
            // Calcular os moments
            Moments mu = moments(contours[i]);
            // Calcular coordenadas dos centroides
            c.centroids[i].x = mu.m10 / mu.m00;
            c.centroids[i].y = mu.m01 / mu.m00;
        }

        //Discriminar centroides
        allCentroids[countOfFrames - 1].resize(contours.size());
        biggestAreas[countOfFrames - 1].resize(contours.size(), 0);
        for (size_t i = 0; i < c.centroids.size(); i++) {
            if (!c.visited[i]) { // Se o centroid nao tiver sido visitado ainda, compare ele com os demais
                double areasSum = contourArea(contours[i]), xSum = (c.centroids[i].x * areasSum), ySum = (c.centroids[i].y * areasSum);
                biggestAreas[countOfFrames - 1][i] += contourArea(contours[i]);
                for (size_t j = i + 1; j < c.centroids.size(); j++) {
                    if (c.visited[j]) continue; // Se o centroid ja foi visitado, pula pro proximo
                    if (twoPointDistance(c.centroids[i], c.centroids[j]) <= ::distance) { // Se a distancia entre os centroides for menor/igual ao valor da distancia, entra
                        double areaToSum = contourArea(contours[j]);
                        // Somando valores para calculo do centroide apropriado
                        areasSum += areaToSum;
                        xSum += (c.centroids[j].x * areaToSum);
                        ySum += (c.centroids[j].y * areaToSum);
                        
                        // Somando valor da area do raio no vetor de areas
                        biggestAreas[countOfFrames - 1][i] += areaToSum;
                        c.visited[j] = 1;
                    }
                }
                Point2f centroidAfterWeight;
                centroidAfterWeight.x = xSum / areasSum;
                centroidAfterWeight.y = ySum / areasSum;
                allCentroids[countOfFrames - 1][i] = centroidAfterWeight;
            }
            c.visited[i] = 1;
        }
        matrixVisited[countOfFrames-1].resize(allCentroids[countOfFrames - 1].size(), 0);

        std::cout << allCentroids[countOfFrames - 1] << endl;
        for (size_t i = 0; i < biggestAreas[countOfFrames - 1].size(); i++)
        {
            std::cout << biggestAreas[countOfFrames-1][i] << " " << allCentroids[countOfFrames-1][i] << endl;

        }

    }

    if (scene == 1) {
        for (size_t k = 1; k < countOfFrames - 1; k++) {
            std::cout << "TO NO FRAME = " << k << endl;

            // Loop de grau 3, para analisar todos os frames, e em cada frame analisar todos os raios dele, e no ultimo loop ficar rodando o codigo ate achar onde o raio acaba
            for (size_t j = 0; j < allCentroids[k].size(); j++) {
                Point2f zero(0,0);
                // Se o raio ja tiver sido visitado antes, ou o centroide for diferente dele mesmo (para os casos de valor NaN) ou se o centroide for igual a zero, apenas va para o proximo
                if (matrixVisited[k][j] == 1 || allCentroids[k][j] != allCentroids[k][j] || allCentroids[k][j] == zero) continue;
                std::cout << "TO NO CONTORNO = " << j << endl;
                int duration = 1;

                // Para calcular o centroide final de um raio, foi considerado todos os frames q ele fez parte, para fazer a media ponderada das areas pelo valor do centroide
                double areasSum = biggestAreas[k][j], xSum = (allCentroids[k][j].x * biggestAreas[k][j]), ySum = (allCentroids[k][j].y * biggestAreas[k][j]);
                Point2f centroide = allCentroids[k][j]; // Centroide atual esta no frame K e no centroide J
                matrixVisited[k][j] = 1; // Registrar como visitado o centroide
                int nextIndex = k + 1; // Index do frame seguinte
                while (true)
                {
                    if (nextIndex == countOfFrames) break; // Caso o index iguale o numero total de frames, saia
                    // O valor da duracao atual do raio e salva, pois, caso ele percorra todos os centroides do frame seguinte e nenhum esteja numa distancia proxima, entao ele nao muda o valor da duracao
                    int beforeCheckDuration = duration;
                    for (size_t i = 0; i < allCentroids[nextIndex].size(); i++)
                    {
                        Point2f nextCentroide = allCentroids[nextIndex][i]; // Centroide do i do frame seguinte
                        if (matrixVisited[nextIndex][i] == 1 || biggestAreas[nextIndex][i] == 0) continue; // Se o centroide i ja foi visitado ou se sua area = 0, pule ele
                        double isInside = twoPointDistance(centroide, nextCentroide);
                        if (isInside <= ::distance) {
                            //Se estiver dentro de distancia do centroide atual, atualizar o centroide
                            std::cout << isInside << " e frame numero: " << nextIndex << " e centroide: " << centroide  << " e nextCentroide:" << nextCentroide << endl;
                            
                            // Somar valores de area e valores ponderados de X e Y do centroide
                            areasSum += biggestAreas[nextIndex][i];
                            xSum += (nextCentroide.x * biggestAreas[nextIndex][i]);
                            ySum += (nextCentroide.y * biggestAreas[nextIndex][i]);
                            centroide = nextCentroide;
                            duration++;

                            //Colocar como visitado
                            matrixVisited[nextIndex][i] = 1;
                            break;
                        }
                    }
                    if (beforeCheckDuration == duration) {
                        // Calcular centroide dividindo os valores de X e Y ponderados pela soma das areas
                        Point2f centroideSum;
                        centroideSum.x = xSum / areasSum;
                        centroideSum.y = ySum / areasSum;
                        // Salvamento da duracao e do valor do centroide em txt
                        fstream newfile;
                        newfile.open("raios.txt", ios::app);
                        newfile << "Inicio = " << k << " e Fim = " << nextIndex-1 << " e Duracao = " << duration << " e Centroide = " << centroideSum << endl;
                        newfile.close();
                        break;
                    }
                    nextIndex++;
                }

            }
        }
    }

    if (scene == 2) {
        // Achar qual o frame com raio de maior area
        int indexOfFrameOfBiggestContour = -1;
        double biggestContourFrames = 0;
        Point2f centroide;
        for (size_t i = 0; i < biggestAreas.size(); i++) {
            for (size_t j = 0; j < biggestAreas[i].size(); j++) {
                if (biggestContourFrames < biggestAreas[i][j]) {
                    indexOfFrameOfBiggestContour = i;
                    centroide = allCentroids[i][j];
                    biggestContourFrames = biggestAreas[i][j];
                }
            }
        }

        printf("Index aqui: %d\n Maior Area aqui: %f\n", indexOfFrameOfBiggestContour, biggestContourFrames);

        int duration = 1;
        int count = 1;
        int nextIndex = indexOfFrameOfBiggestContour + count;
        while (true)
        {
            if (nextIndex == countOfFrames) break; // Caso o index ultrapasse o numero de frames, saia
            // O valor da duracao atual do raio e salva, pois, caso ele percorra todos os centroides do frame seguinte e nenhum esteja numa distancia proxima, entao ele nao muda o valor da duracao
            int beforeCheckDuration = duration;
            for (size_t i = 0; i < allCentroids[nextIndex].size(); i++)
            {
                Point2f nextCentroide = allCentroids[nextIndex][i]; // Centroide do i do frame seguinte
                if (matrixVisited[nextIndex][i] == 1 || biggestAreas[nextIndex][i] == 0) continue; // Se o centroide i ja foi visitado ou se sua area = 0, pule ele
                double isInside = twoPointDistance(centroide, nextCentroide);
                if (isInside <= ::distance) {
                    centroide = nextCentroide;
                    duration++;

                    //Colocar como visitado
                    matrixVisited[nextIndex][i] = 1;
                    break;
                }
            }
            if (beforeCheckDuration == duration) {
                // Salvamento da duracao
                if (count == -1){
                    std::cout << "Duracao = " << duration << endl;
                    break;
                }
                nextIndex = indexOfFrameOfBiggestContour;
                count = -1;
            }
            nextIndex += count;
        }

    }

    waitKey(0);

    return 0;
}
