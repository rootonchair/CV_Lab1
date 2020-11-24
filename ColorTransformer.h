﻿#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const cv::Mat& sourceImage, cv::Mat& destinationImage, short b) 
	{

		if (sourceImage.channels() == 3)  // rgb image
			destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);
		else if (sourceImage.channels() == 1)
			destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		else
			return 0;

		//truy xuất đến tọa độ ảnh (i,j)
		//image.at<uchar>(j, i) = 255;
		for (int i = 0; i < sourceImage.rows; i++)
		{
			const uchar* rowOrgData = sourceImage.ptr<uchar>(i);
			uchar* rowDstData = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < sourceImage.cols * sourceImage.channels(); j++)
			{
				if (rowOrgData[j] + b <= 255 && rowOrgData[j] + b >= 0)
					rowDstData[j] = rowOrgData[j] + b;
				else
				{
					if (rowOrgData[j] + b > 255)
						rowDstData[j] = 255;
					if (rowOrgData[j] + b < 0)
						rowDstData[j] = 0;
				}

			}
		}

		return 1;
	}

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/	
	int ChangeContrast(const cv::Mat& sourceImage, cv::Mat& destinationImage, float c) 
	{


		if (sourceImage.channels() == 3)  // rgb image
			destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);
		else if (sourceImage.channels() == 1)
			destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		else
			return 0;

		//truy xuất đến tọa độ ảnh (i,j)
		//image.at<uchar>(j, i) = 255;
		for (int i = 0; i < sourceImage.rows; i++)
		{
			const uchar* rowOrgData = sourceImage.ptr<uchar>(i);
			uchar* rowDstData = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < sourceImage.cols * sourceImage.channels(); j++)
			{
				if (rowOrgData[j] * c <= 255 && rowOrgData[j] * c >= 0)
					rowDstData[j] = rowOrgData[j] * c;
				else
				{
					if (rowOrgData[j] * c > 255)
						rowDstData[j] = 255;
					if (rowOrgData[j] * c < 0)
						rowDstData[j] = 0;
				}

			}
		}

		return 1;
	}
	
	
	/*
	Hàm tính lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		histMatrix : ma trận histogram (nChannels x 256), mỗi dòng là 1 mảng 256 phần tử histogram của từng kênh màu
	Hàm trả về:
		1: Nếu thành công thì trả về matrix kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tính được histogram hoặc ảnh input không tồn tại
	*/
	int CalcHistogram(const cv::Mat& sourceImage, cv::Mat& histMatrix);
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		image : ảnh dùng để tính histogram
		histImage : ảnh histogram
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int HistogramEqualization(const cv::Mat& sourceImage, cv::Mat& destinationImage);
	


		/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		histMatrix : ma trận histogram đã tính được
		histImage : ảnh histogram được vẽ
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int DrawHistogram(const cv::Mat& histMatrix, cv::Mat& histImage);

	
	/*
	Hàm so sánh hai ảnh
	Tham so :
		image1 : ảnh thứ nhất
		image2 : ảnh thứ hai
	Hàm trả về:
		độ đo sự tương đồng giữa hai ảnh
	*/
	float CompareImage(const cv::Mat& image1, cv::Mat& image2);
	
	
	ColorTransformer() 
	{

	}

	~ColorTransformer() 
	{

	}
};

