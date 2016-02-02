//============================================================================
// Name        : Color.cpp
// Author      : Alvaro
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ColorLib.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <vector>


using namespace std;
using namespace cv;



std::vector<Mat> GetASampleOfFrames(int id_camara,int numberOfSamples)
{
std::vector<Mat>Imgs;
VideoCapture cap(id_camara);

if (!cap.isOpened())
{
	cerr << "Could not open camera." << endl;
    exit(EXIT_FAILURE);
}


for(int x=0;x<=numberOfSamples;x++) //Capturamos el numero de frames solicitados
{
	Mat frame;
	cap >> frame;
	Imgs.push_back(frame); //Guarda el frame capturado
}

return Imgs;
}

int main() {
	ColorLib colorLib;
	//vector<Mat>frames = GetASampleOfFrames(1,1);
	colorLib.detectColor(0);

	waitKey(0);
	return 0;
}
