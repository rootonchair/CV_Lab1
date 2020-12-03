#pragma once
#include "opencv2/opencv.hpp"
#include <math.h>
#include "opencv2/highgui.hpp"

#define R 2
#define G 1
#define B 0
#define H 0
#define S 1
#define V 2
using namespace std;

double max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}
double min(double a, double b)
{
	if (a < b)
		return a;
	return b;
}
void SetValuePixel(cv::Mat& img, int row, int col, int color, int data)
{
	img.data[row * img.channels() * img.cols + col * img.channels() + color] = data;
}
int GetPixelValue(cv::Mat img, int row, int col, int color)
{
	return img.data[row * img.channels() * img.cols + col*img.channels() + color];
}
class Converter
{
public:
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

		destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		
		//(*dstPixel) = 0.299*currentPixel[R] + 0.587*currentPixel[G] + 0.114*currentPixel[B];
		for (int row= 0; row < sourceImage.rows; row++)
			for (int col = 0; col < sourceImage.cols; col++)
			{
				int grayColor = 0.299 * GetPixelValue(sourceImage, row, col, R) + 0.558 * GetPixelValue(sourceImage, row, col, G) + 0.114 * GetPixelValue(sourceImage, row, col, B);
				SetValuePixel(destinationImage, row, col, 0, grayColor);
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
				uchar currentPixel = GetPixelValue(sourceImage, row, col, 0);
				SetValuePixel(destinationImage, row, col, R, currentPixel);
				SetValuePixel(destinationImage, row, col, G, currentPixel);
				SetValuePixel(destinationImage, row, col, B, currentPixel);
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
				cv::Vec3b currentPixel = sourceImage.at<cv::Vec3b>(row, col);
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

				//destinationImage.at<cv::Vec3b>(row, col)[H] = (uchar)h;
				SetValuePixel(destinationImage, row, col, H, h);
				SetValuePixel(destinationImage, row, col, S, s);
				SetValuePixel(destinationImage, row, col, V, v);
				//cout << h << " " << s << " " << v << endl;
				///destinationImage.at<cv::Vec3b>(row, col)[S] = (uchar)s;
				//destinationImage.at<cv::Vec3b>(row, col)[V] = (uchar)v;

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
	int HSV2RGB(const cv::Mat& sourceImage, cv::Mat& destinationImage)
	{
		if (sourceImage.channels() != 3)
			return 1;

		destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);

		for (int row = 0; row < sourceImage.rows; row++)
			for (int col = 0; col < sourceImage.cols; col++)
			{
				double r, g, b;
				double h, s, v;
				h = max(0, min(255, GetPixelValue(sourceImage, row, col, H))) * 2.0;
				s = max(0, min(255, GetPixelValue(sourceImage, row, col, S))) / 255.0;
				v = max(0, min(255, GetPixelValue(sourceImage, row, col, V))) / 255.0;
				//cout << h << " " << s << " " << v << endl;
				int hi = int(h / 60) % 6;
				double f = h / 60 - hi;
				double p = v * (1 - s);
				double q = v * (1 - f * s);
				double t = v * (1 - s + s * f);
				switch (hi) 
				{
				case 0:
					r = v;
					g = t;
					b = p;
					break;
				case 1:
					r = q;
					g = v; 
					b = p;
					break;
				case 2:
					r = p;
					g = v;
					b = t;
					break;
				case 3:
					r = q;
					g = q;
					b = v;
					break;
				case 4: 
					r = t;
					g = p;
					b = v;
					break;
				default:
					r = v;
					g = p;
					b = q;
					break;
				}

				SetValuePixel(destinationImage, row, col, R, int(r / 2)) ;
				SetValuePixel(destinationImage, row, col, G, int(g*255));
				SetValuePixel(destinationImage, row, col, B, int(b*255));
			}
		return 0;
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
		if (sourceImage.data == NULL)
			return 1;

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

