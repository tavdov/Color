/*
 * ColorLib.cpp
 *
 *  Created on: 01/02/2016
 *      Author: Alvaro
 */

#include "ColorLib.h"

#include <iostream>
#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv_modules.hpp"
# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/nonfree/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>


using namespace std;
using namespace cv;

char key;
//MORPH_CROSS	MORPH_RECT	MORPH_ELLIPSE
/// Global variables
Mat erosion_dst, dilation_dst;
Mat canny_output;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
void Erosion( int, void* );
void Dilation( int, void* );

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges, canny;
Mat morphology;

int edgeThresh = 1;
int lowThreshold = 15;
int const max_lowThreshold = 100;
int const max_Size = 6;
int Gaus_Size  = 3;
int ratio = 2;
int kernel_size = 3;
//char* window_name = "Edge Map";
RNG rng(12345);


vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */

/**  @function Erosion  */
void Erosion( int, void* )
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( dilation_dst, erosion_dst, element );
  morphology = erosion_dst;
  imshow( "Morphology", erosion_dst );
}

/** @function Dilation */
void Dilation( int, void* )
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( canny_output, dilation_dst, element );
  //imshow( "Morphology", dilation_dst );
}

void CannyThreshold(int, void*)
//void CannyThreshold(void)
{
	//El Gaus_Size debe ser impar y diferente de 0
	Gaus_Size = Gaus_Size/1;
	if (Gaus_Size == 0)
		Gaus_Size = 1;
	if (!(Gaus_Size%2))
		Gaus_Size++;

	/// Reduce noise with a kernel 3x3
	/// Se hace blur para perder detalle y sea mas facil la deteccion de los contornos
	GaussianBlur( src_gray, detected_edges, Size(Gaus_Size, Gaus_Size), 0 );
/*
	//OpenCV use H: 0 - 180, S: 0 - 255, V: 0 - 255 ---> En ese orden
	Mat chan[3];
	split (detected_edges,chan);
	Mat bin_red = ((chan[0]<80)&(chan[0]>29)); 			// note: single '|'
	Mat bin_grn = ((chan[1]<20)&(chan[1]>0));
	Mat bin_blu = ((chan[2]<220)&(chan[2]>170));

	Mat bin = bin_red & bin_grn & bin_blu;  			// where all conditions were met

	 /// Show in a window
	 namedWindow( "Gris & Blur", CV_WINDOW_AUTOSIZE );
	 imshow( "Gris & Blur", bin );
*/

	/// Canny detector
	Canny( detected_edges, canny_output, lowThreshold, lowThreshold*ratio, kernel_size );
	canny_output.convertTo(canny_output, CV_8U);

	/*

	// Mas informacion sobre getStructuringElement
	// http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html?highlight=getstructuringelement#getstructuringelement
	//Mat kernel = getStructuringElement( dilation_type,Size( 2*dilation_size + 1, 2*dilation_size+1));
	cv::Mat kernel = cv::Mat::ones(5, 5, CV_8U);
	Mat kernel2 = getStructuringElement( erosion_type,Size( 2*erosion_size + 1, 2*erosion_size+1 ));
	cv::Mat morphology;

	cv::dilate(canny_output, morphology, kernel,Point(-1,-1), 2);	// Se elimina ruido
	//cv::erode(canny_output, morphology, kernel, Point(-1,-1), 10);		// Se elimina ruido

	*/
 }

Mat img_original;
Mat proceso;

ColorLib::ColorLib() {
	// TODO Auto-generated constructor stub
	cout<<"\nIniciando la Busqueda de color "<<endl;

}

ColorLib::~ColorLib() {
	// TODO Auto-generated destructor stub
	cout<<"\nColor detectado"<<endl;
}


void ColorLib::detectColor(int ID)
{
	//Tomar la foto
		VideoCapture cap(1); 				//Abrir la cámara conectada
		if(!cap.isOpened())  				//Verificar si no hay error
			//continue;						//Detecta error

		Mat src;							//Crear la matriz para guardar la captura

	while((char)(key) != 27)
	{

		cap>>src; 						//Tomar el frame de la cámara

		// Hacer un marco de color negro
		copyMakeBorder( src, src, 1, 1, 1, 1, BORDER_CONSTANT, 0 );

		/// Create a matrix of the same type and size as src (for dst)
		dst.create( src.size(), src.type() );

		/// Convert the image to grayscale
		cvtColor( src, src_gray, CV_BGR2GRAY );

		/// Create a window
		namedWindow( "Edge Map", CV_WINDOW_AUTOSIZE );
		//CannyThreshold();

		/// Create a Trackbar for user to enter threshold
		createTrackbar( "Min Threshold:", "Edge Map", &lowThreshold, max_lowThreshold, CannyThreshold );

		/// Create a Trackbar for user to enter Gaussian Size
		createTrackbar( "Gaussian Size:", "Edge Map", &Gaus_Size, max_Size, CannyThreshold );

		/// Show the image
		CannyThreshold(0, 0);

		namedWindow( "Morphology", CV_WINDOW_AUTOSIZE );

		  /// Create Dilation Trackbar
		  createTrackbar( "Dilate Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Morphology",
		                  &dilation_elem, max_elem,
		                  Dilation );

		  createTrackbar( "Dilate Kernel size:\n 2n +1", "Morphology",
		                  &dilation_size, max_kernel_size,
		                  Dilation );
		  /// Create Erosion Trackbar
		  createTrackbar( "Erode Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Morphology",
			               &erosion_elem, max_elem,
			               Erosion );

		  createTrackbar( "Erode Kernel size:\n 2n +1", "Morphology",
			               &erosion_size, max_kernel_size,
			               Erosion );


		  /// Default start
		  Dilation( 0, 0 );
		  Erosion( 0, 0 );

		  findContours(morphology, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );		//Busca contornos externos
		  unsigned int cont_area = 1;
		  unsigned int i;

		  /// Draw contours
		  Mat drawing = Mat::zeros( morphology.size(), CV_8UC3 );


		  for( i = 0; i< contours.size(); i++ )
		  {
				//Contornos mayores a un area definida, para quitar detalles
				//if ( contourArea(contours[i]) > 10000  && contourArea(contours[i]) < 300000)
			  if (contourArea(contours[i]) < 250000)
				{
					Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					// Mas informacion sobre drawContours
					// http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=drawcontours#drawcontours
					drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
					if ( contourArea(contours[i]) > contourArea(contours[cont_area]))
						{
							cont_area = i;
						}
				}
			 }
			//Area mayor
			//cout<< "Area" << cont_area << ": "<< contourArea(contours[cont_area]) << endl;

		  /// Show in a window
		  			 namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
		  			 imshow( "Contours", drawing );

			Mat mask = Mat::zeros(src.rows, src.cols, CV_8U);				// Mascara de todo negro
			Mat resultado;
			drawContours(mask, contours, cont_area, 255, CV_FILLED, 8);			// Dibujar en la mascara anterior solo el contorno mayor

			Mat centro;
			mask(Rect((src.cols/2) -50 , (src.rows/2) - 50 ,100,100)).copyTo(centro);
			Scalar promedio = mean(centro);
			//cout << "Mean " << promedio[0] << endl;


			 /// Show in a window
			 namedWindow( "Mascara", CV_WINDOW_AUTOSIZE );
			 imshow( "Mascara", mask );


			 if(promedio[0] > 125)
			 {

				 /// Convert the image to grayscale
				 Mat src_hsv;
				 cvtColor( src, src_hsv, CV_BGR2HSV );
				 //Aplicar una mascara a la imagen original para estudiar su color
				 src_hsv.copyTo( resultado, mask);

				 Scalar promedio2 = mean(resultado);
				 /// Show in a window
				 namedWindow( "Figura mayor", CV_WINDOW_AUTOSIZE );
				 imshow( "Figura mayor", resultado );
				 long temp = countNonZero(mask);
				 long pixeles = (resultado.cols * resultado.rows);
				 /*
				 for(i = 0; i< src.rows; i++)
				 {
					 for (int j = 0; j < src.cols; j++)
					 {
						 Vec3b color = resultado.at<Vec3b>(i, j);
						 if (color [0] != 0 && color [1] != 0 && color [2] != 0)
						 {
							 H += color[0];
							 S += color[1];
							 V += color[2];
						 }
					 }
				 }
				 */
				 if (temp != 0)
				 {
					 /*
					 H = H/temp;
				 	 S = S/temp;
				 	 V = V/temp;
				 	 */
				 	 cout << "H " << ((promedio2[0]*pixeles)/temp) << " S " << ((promedio2[1]*pixeles)/temp) << " V " << ((promedio2[2]*pixeles)/temp) << " Pixeles - "<<  temp << endl;
				 }
			 }

			 /// Using Canny's output as a mask, we display our result
			 dst = Scalar::all(0);

			src.copyTo( dst, canny_output);
			imshow( "Edge Map", dst );


		key = cvWaitKey(10);     //Capture Keyboard stroke
	}
}
