#include <iostream>
#include <cstring>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "ColorTransformer.h"
#include "Converter.h"
#include "TypeConvert.h"

using namespace cv;

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 3)
			throw std::string("Invalid Syntax");

		char* command = argv[1];
		char* inputPath = argv[2];

		Mat orgImage;
		Mat resultImage;
		int result;

		if (strcmp(command, "--rgb2gray") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			Converter converter;
			result = converter.Convert(orgImage, resultImage, 0);
		}
		else if (strcmp(command, "--gray2rgb") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_GRAYSCALE);
			Converter converter;
			result = converter.Convert(orgImage, resultImage, 1);
		}
		else if (strcmp(command, "--rgb2hsv") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			Converter converter;
			result = converter.Convert(orgImage, resultImage, 2);
		}
		else if (strcmp(command, "--hsv2rgb") == 0) // !!!
		{
			orgImage = imread(inputPath, COLOR_RGB2HSV);
			Converter converter;
			result = converter.Convert(orgImage, resultImage, 3);
		}
		else if (strcmp(command, "--bright") == 0)
		{
			if (argc < 4)
				throw std::string("Invalid Syntax");

			short b = (short)ParseInt(argv[3]);

			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			ColorTransformer cTrans;
			result = cTrans.ChangeBrighness(orgImage, resultImage, b);
		}
		else if (strcmp(command, "--contrast") == 0)
		{
			if (argc < 4)
				throw std::string("Invalid Syntax");

			float c = (float)ParseDouble(argv[3]);

			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			ColorTransformer cTrans;
			result = cTrans.ChangeContrast(orgImage, resultImage, c);
		}
		else if (strcmp(command, "--hist") == 0)
		{

		}
		else if (strcmp(command, "--equalhist") == 0)
		{
			if (argc > 3 && strcmp(argv[3], "-g") == 0)
				orgImage = imread(inputPath, CV_LOAD_IMAGE_GRAYSCALE);
			else
				orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);

			ColorTransformer cTrans;
			cTrans.HistogramEqualization(orgImage, resultImage);
		}
		else if (strcmp(command, "--drawhist") == 0)
		{
			if (argc > 3 && strcmp(argv[3], "-g") == 0)
				orgImage = imread(inputPath, CV_LOAD_IMAGE_GRAYSCALE);
			else
				orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);

			ColorTransformer cTrans;
			Mat hist;
			cTrans.CalcHistogram(orgImage, hist);
			cTrans.DrawHistogram(hist, resultImage);
		}
		else if (strcmp(command, "--compare") == 0)
		{

		}
		else
		{
			throw std::string("No such command ") + command;
		}

		//if (result != 0) {
		//	throw std::string("Cannot perform operation. Maybe due to unexisted image or invalid command arguments");
		//}

		namedWindow("Original Image");
		imshow("Original Image", orgImage);
		namedWindow("Result");
		imshow("Result", resultImage);
		waitKey(0);
	}
	catch (std::string errMsg) {
		std::cout << errMsg;
	}

	return 0;
}