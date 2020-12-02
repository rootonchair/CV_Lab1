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
		bool result = false;

		if (strcmp(command, "--rgb2gray") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			Converter converter;
			if (converter.Convert(orgImage, resultImage, 0) == 0)
				result = true;
		}
		else if (strcmp(command, "--gray2rgb") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_GRAYSCALE);
			Converter converter;
			if (converter.Convert(orgImage, resultImage, 1) == 0)
				result = true;
		}
		else if (strcmp(command, "--rgb2hsv") == 0)
		{
			orgImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			Converter converter;
			if (converter.Convert(orgImage, resultImage, 2) == 0)
				result = true;
		}
		else if (strcmp(command, "--hsv2rgb") == 0)
		{
			Mat rgbIm;
			rgbIm = imread(inputPath, CV_LOAD_IMAGE_COLOR);
			if (rgbIm.data != NULL)
				cvtColor(rgbIm, orgImage, COLOR_BGR2HSV);

			Converter converter;
			if (converter.Convert(orgImage, resultImage, 3) == 0)
				result = true;
		}
		else if (strcmp(command, "--bright") == 0)
		{
			if (argc < 4)
				throw std::string("Invalid Syntax");

			short b = (short)ParseInt(argv[3]);

			orgImage = imread(inputPath, IMREAD_ANYCOLOR);
			ColorTransformer cTrans;
			if (cTrans.ChangeBrighness(orgImage, resultImage, b) == 1)
				result = true;
		}
		else if (strcmp(command, "--contrast") == 0)
		{
			if (argc < 4)
				throw std::string("Invalid Syntax");

			float c = (float)ParseDouble(argv[3]);

			orgImage = imread(inputPath, IMREAD_ANYCOLOR);
			ColorTransformer cTrans;
			if (cTrans.ChangeContrast(orgImage, resultImage, c) == 1)
				result = true;
		}
		else if (strcmp(command, "--hist") == 0)
		{
			orgImage = imread(inputPath, IMREAD_ANYCOLOR);
			
			ColorTransformer cTrans;
			Mat hist;
			if (cTrans.CalcHistogram(orgImage, hist) == 1)
				result = true;

			for (int i = 0; i < hist.rows; i++)
			{
				uint* RowData = hist.ptr<uint>(i);
				std::cout << "[ row = " << i << "] \n";
				for (int j = 0; j < hist.cols; j++)
				{
					std::cout << "[" << i << ":" << j << "]: " << RowData[j] << "\n";
				}
				std::cout << std::endl;
			}
		}
		else if (strcmp(command, "--equalhist") == 0)
		{
			orgImage = imread(inputPath, IMREAD_ANYCOLOR);

			ColorTransformer cTrans;
			if (cTrans.HistogramEqualization(orgImage, resultImage) == 1)
				result = true;
		}
		else if (strcmp(command, "--drawhist") == 0)
		{
			orgImage = imread(inputPath, IMREAD_ANYCOLOR);

			ColorTransformer cTrans;
			Mat hist;
			if (cTrans.CalcHistogram(orgImage, hist) == 1)
				result = true;
			if (result && cTrans.DrawHistogram(hist, resultImage) == 1)
				result = true;
			else
				result = false;
		}
		else if (strcmp(command, "--compare") == 0)
		{
			throw std::string("This feature is under maintenance");
		}
		else
		{
			throw std::string("No such command ") + command;
		}

		if (!result) {
			throw std::string("Cannot perform operation. Maybe due to unexisted image or invalid command arguments");
		}

		namedWindow("Original Image");
		imshow("Original Image", orgImage);
		if (resultImage.data != NULL)
		{
			namedWindow("Result");
			imshow("Result", resultImage);
		}
		waitKey(0);
	}
	catch (std::string errMsg) {
		std::cout << errMsg;
	}

	return 0;
}