#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#define R 2
#define G 1
#define B 0
#define H 0
#define S 1
#define V 2


class Converter
{
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
	sourceImage: ảnh input
	destinationImage: ảnh output	
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2GrayScale(const cv::Mat& sourceImage, cv::Mat& destinationImage) 
	{
		if (sourceImage.channels() != 3)
			return 1;

		int srcWidthStep = sourceImage.step[0];
		int srcChannels = sourceImage.step[1];

		destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		int dstWidthStep = destinationImage.step[0];
		int dstChannel = destinationImage.step[1];

		for (int row = 0; row < sourceImage.rows; row++) 
		{
			for (int col = 0; col < sourceImage.cols; col++) 
			{
				uchar* currentPixel = sourceImage.data + srcWidthStep * row + srcChannels * col;
				uchar* dstPixel = destinationImage.data + dstWidthStep * row + dstChannel * col;
				(*dstPixel) = 0.299*currentPixel[R] + 0.587*currentPixel[G] + 0.114*currentPixel[B];
			}
		}
		return 0;
	}

	/*
	Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int GrayScale2RGB(const cv::Mat& sourceImage, cv::Mat& destinationImage) 
	{
		if (sourceImage.channels() != 1)
			return 1;
		destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);

		for (int row = 0; row < sourceImage.rows; row++) 
		{
			for (int col = 0; col < sourceImage.cols; col++) 
			{
				uchar currentPixel = sourceImage.ptr<uchar>(row)[col];
				uchar* dstPixel = destinationImage.ptr<uchar>(row) + col * destinationImage.channels();
				dstPixel[R] = currentPixel;
				dstPixel[G] = currentPixel;
				dstPixel[B] = currentPixel;
			}
		}

		return 0;
	}
	
	double findMax(double r, double g, double b) 
	{
		double firstMax = (r > g ? r : g);
		return (firstMax > b ? firstMax : b);
	}

	double findMin(double r, double g, double b) 
	{
		double firstMin = (r > g ? g : r);
		return (firstMin > b ? b : firstMin);
	}

	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const cv::Mat& sourceImage, cv::Mat& destinationImage) 
	{
		if (sourceImage.channels() != 3)
			return 1;

		destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);

		for (int row = 0; row < sourceImage.rows; row++) 
		{
			for (int col = 0; col < sourceImage.cols; col++) 
			{
				const uchar* currentPixel = sourceImage.ptr<uchar>(row) + col * sourceImage.channels();
				double h, s, v;

				double r_scaled = currentPixel[R] / 255.0;
				double g_scaled = currentPixel[G] / 255.0;
				double b_scaled = currentPixel[B] / 255.0;

				double val_max = findMax(r_scaled, g_scaled, b_scaled);
				double val_min = findMin(r_scaled, g_scaled, b_scaled);
				double delta = val_max - val_min;

				v = val_max * 255;
				if (delta < 0.00001) 
				{
					h = 0.0;
					s = NAN;
				}
				else if (val_max <= 0.0) 
				{
					s = 0.0;
					h = NAN;
				}
				else 
				{
					s = delta / val_max;
					if (r_scaled >= val_max) {
						h = (g_scaled - b_scaled) / delta;
					}
					else if (g_scaled >= val_max) {
						h = 2.0 + (b_scaled - r_scaled) / delta;
					}
					else {
						h = 4.0 + (r_scaled - g_scaled) / delta;
					}
					h *= 60.0;
					if (h < 0.0)
						h += 360;

					//Turn to OpenCV style
					s *= 255.0;
					h /= 2.0;
				}

				uchar* dstPixel = destinationImage.ptr<uchar>(row) + col * destinationImage.channels();
				dstPixel[H] = (uchar)h;
				dstPixel[S] = (uchar)s;
				dstPixel[V] = (uchar)v;

			}
		}

		return 0;
	}
	
	
	
	/*
	Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int HSV2RGB(const cv::Mat& sourceImage, cv::Mat& destinationImage) {
		return 1;
	}
	
	
	
public:
	/*
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
		0,1: chuyển từ RGB sang GrayScale và ngược lại
		2,3: chuyển từ RGB sang HSV và ngược lại		
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
	int Convert(cv::Mat& sourceImage, cv::Mat& destinationImage, int type) 
	{
		switch (type)
		{
		case 0:
			return RGB2GrayScale(sourceImage, destinationImage);
		case 1:
			return GrayScale2RGB(sourceImage, destinationImage);
		case 2:
			return RGB2HSV(sourceImage, destinationImage);
		case 3:
			return HSV2RGB(sourceImage, destinationImage);
		default:
			return 1;
		}
	}

	int Convert(IplImage* sourceImage, IplImage* destinationImage, int type) 
	{
		cv::Mat org = cv::cvarrToMat(sourceImage);
		cv::Mat dst = cv::cvarrToMat(destinationImage);
		return Convert(org, dst, type);
	}

	Converter() 
	{

	}
	~Converter() 
	{

	}
};

