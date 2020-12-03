#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include"Converter.h"

using namespace std;
using namespace cv;

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
		if (sourceImage.data == NULL)
		{
			return 0;
		}

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
		if (sourceImage.data == NULL)
		{
			return 0;
		}

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
	int CalcHistogram(const cv::Mat& sourceImage, cv::Mat& histMatrix)
	{
		if (sourceImage.data == NULL)
		{
			return 0;
		}
		// nChannels=1 nếu là ảnh xám, nChannels=3 nếu là ảnh màu
		int nChannels = sourceImage.channels();
		// Khởi tạo ma trận histogram (nChannel x 256), giá trị mặc định là 0
		histMatrix = Mat(nChannels, 256, CV_32SC1, Scalar(0));
		// duyệt qua các dòng pixel của ảnh sourceImage
		for (int i = 0; i < sourceImage.rows; i++)
		{
			// lấy con trỏ đầu dòng của sourceImage
			const uchar* RowData = sourceImage.ptr<uchar>(i);

			for (int j = 0; j < sourceImage.cols; j++, RowData += nChannels)
			{
				for (int n = 0; n < nChannels; n++)
				{
					// lấy con trỏ đầu dòng (từng kênh màu) của histMatrix
					uint* hist_1n = histMatrix.ptr<uint>(n);
					hist_1n[RowData[n]]++;
				}
			}
		}
		return 1;
		
	}
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		image : ảnh dùng để tính histogram
		histImage : ảnh histogram
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int HistogramEqualization(const cv::Mat& sourceImage, cv::Mat& destinationImage)
	{
		if (sourceImage.empty())
		{
			return 0;
		}

		int channel, height, width, row, col, i;
		channel = sourceImage.channels();
		height = sourceImage.rows;
		width = sourceImage.cols;		

		if (channel > 1) // anh RGB
		{			
			Mat imgB(sourceImage.size(), CV_8UC1);
			Mat imgG(sourceImage.size(), CV_8UC1);
			Mat imgR(sourceImage.size(), CV_8UC1);

			destinationImage = sourceImage.clone();
			int srcChannels = sourceImage.channels();
			int dstChannels = destinationImage.channels();
			int imgChannels = imgB.channels();

			//truy xuat den tung kenh mau
			for (row = 0; row < height; row++)
			{
				const uchar* pSrcRow = sourceImage.ptr<uchar>(row);
				uchar* pImgBRow = imgB.ptr<uchar>(row);
				uchar* pImgRRow = imgR.ptr<uchar>(row);
				uchar* pImgGRow = imgG.ptr<uchar>(row);

				for (col = 0; col < width; col++, pSrcRow += srcChannels, pImgBRow += imgChannels, pImgGRow += imgChannels, pImgRRow += imgChannels)
				{
					pImgBRow[0] = pSrcRow[0];
					pImgGRow[0] = pSrcRow[1];
					pImgRRow[0] = pSrcRow[2];
				}
			}
			//can bang histogram cho tung kenh mau
			HistogramEqualization(imgB, imgB);
			HistogramEqualization(imgG, imgG);
			HistogramEqualization(imgR, imgR);

			//gan lai gia tri cho anh dich
			for (row = 0; row < height; row++)
			{
				uchar* pDstRow = destinationImage.ptr<uchar>(row);
				uchar* pImgBRow = imgB.ptr<uchar>(row);
				uchar* pImgRRow = imgR.ptr<uchar>(row);
				uchar* pImgGRow = imgG.ptr<uchar>(row);

				for (col = 0; col < width; col++, pDstRow += srcChannels, pImgBRow += imgChannels, pImgGRow += imgChannels, pImgRRow += imgChannels)
				{

					pDstRow[0] = pImgBRow[0];
					pDstRow[1] = pImgGRow[0];
					pDstRow[2] = pImgRRow[0];
				}
			}
		}
			
		else // anh xam
		{				
			vector<int> hist_Arr(256, 0);
			int srcChannels = sourceImage.channels();
			int dstChannels = destinationImage.channels();

			//tinh so luong diem anh cua tung do sang
			for (row = 0; row < height; row++)
			{
				const uchar* pSrcRow = sourceImage.ptr<uchar>(row);
				for (col = 0; col < width; col++, pSrcRow += srcChannels)
				{
					hist_Arr[pSrcRow[0]] += 1;
				}
			}

			//tinh tong so diem anh tu 0 den k
			vector<int> sum_of_hist(256);
			sum_of_hist[0] = hist_Arr[0];

			for (i = 1; i < 256; i++)
			{
				sum_of_hist[i] = sum_of_hist[i - 1] + hist_Arr[i];
			}

			// Tinh do sang moi cho tung do sang
			double average = (double)255 / (height * width);
			for (i = 0; i < 256; i++)
			{
				sum_of_hist[i] = round(average * sum_of_hist[i]);
			}

			//Gan vao anh dich
			destinationImage = sourceImage.clone();

			for (row = 0; row < height; row++)
			{
				const uchar* pSrcRow = sourceImage.ptr<uchar>(row);
				uchar* pDstRow = destinationImage.ptr<uchar>(row);
				for (col = 0; col < width; col++, pSrcRow += srcChannels, pDstRow += srcChannels)
				{
					pDstRow[0] = saturate_cast<uchar>(sum_of_hist[pSrcRow[0]]);
				}
			}
		}

		return 1;	
	}
	


	/*
	Hàm vẽ lược đồ màu cho ảnh bất kỳ
	Tham so :
		histMatrix : ma trận histogram đã tính được
		histImage : ảnh histogram được vẽ
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int DrawHistogram(const cv::Mat& histMatrix, cv::Mat& histImage)
	{
		const int hWidth = 256;
		const int hHeight = 256;
		const int spaceFromTop = 64;

		// Khởi tạo ảnh histogram
		histImage = cv::Mat(hHeight, hWidth * histMatrix.rows, CV_8UC3);
		for (int i = 0; i < histImage.rows; ++i)
		{
			uchar* arr = histImage.ptr<uchar>(i);
			for (int j = 0; j < histImage.cols * histImage.channels(); ++j)
				arr[j] = 0;
		}

		// Vẽ từng channel của histogram
		if (histMatrix.rows == 1) // Ảnh greyscale
		{
			const uint* hist_1C = histMatrix.ptr<uint>(0);
			int window[] = { spaceFromTop, 0, hHeight - 1, hWidth - 1 };
			if (!DrawHistogram_1C(hist_1C, cv::Scalar(255, 255, 255), histImage, window))
				return 0;
		}
		else if (histMatrix.rows == 3) // Ảnh RGB
		{
			// Vẽ kênh B
			const uint* hist_1C = histMatrix.ptr<uint>(0);
			int windowB[] = { spaceFromTop, 0, hHeight - 1, hWidth - 1 };
			if (!DrawHistogram_1C(hist_1C, cv::Scalar(255, 0, 0), histImage, windowB))
				return 0;

			// Vẽ kênh G
			hist_1C = histMatrix.ptr<uint>(1);
			int windowG[] = { spaceFromTop, hWidth, hHeight - 1, 2 * hWidth - 1 };
			if (!DrawHistogram_1C(hist_1C, cv::Scalar(0, 255, 0), histImage, windowG))
				return 0;

			// Vẽ kênh R
			hist_1C = histMatrix.ptr<uint>(2);
			int windowR[] = { spaceFromTop, 2 * hWidth, hHeight - 1, 3 * hWidth - 1 };
			if (!DrawHistogram_1C(hist_1C, cv::Scalar(0, 0, 255), histImage, windowR))
				return 0;
		}
		else
			return 0;

		return 1;
	}

	
	/*
	Hàm so sánh hai ảnh
	Tham so :
		image1 : ảnh thứ nhất
		image2 : ảnh thứ hai
	Hàm trả về:
		độ đo sự tương đồng giữa hai ảnh
	*/
	float CompareImage(const cv::Mat& image1, cv::Mat& image2) 
	{

		//So sánh hai ảnh xám
		if (image1.channels() == 1 && image2.channels() == 1)
		{
			vector<float> his1(256, 0), his2(256, 0);
			//Tính Histogram của ảnh 1
			for (int row = 0; row < image1.rows; row++)
				for (int col = 0; col < image1.cols; col++)
				{
					his1[GetPixelValue(image1, row, col, 0)] += 1;
				}

			for (int i = 0; i < 256; i++)
				his1[i] /= (1.0 * image1.rows * image1.cols);

			//Tính Histogram của ảnh 2
			for (int row = 0; row < image2.rows; row++)
				for (int col = 0; col < image2.cols; col++)
				{
					his2[GetPixelValue(image2, row, col, 0)] += 1;
				}

			for (int i = 0; i < 256; i++)
				his2[i] /= (1.0 * image2.rows * image2.cols);

			//So Sánh hai histogram 
			// Công thức sum(abs(his1[i] - his2[i]))
			float percent_diff = 0.0;
			for (int i = 0; i < 256; i++)
			{
				if (his1[i] == 0 && his2[i] == 0)
					continue;
				percent_diff = percent_diff + (his1[i] - his2[i])*(his1[i] - his2[i]) / (his1[i] + his2[i]);
			}

			return sqrtf(percent_diff);

		}

		//so sánh hai ảnh màu
		if (image1.channels() == 3 && image2.channels() == 3)
		{
			vector<float> his1_R(256, 0), his2_R(256, 0), his1_G(256, 0), his2_G(256, 0), his1_B(256, 0), his2_B(256, 0);
			//Tính Histogram của ảnh 1
			for (int row = 0; row < image1.rows; row++)
				for (int col = 0; col < image1.cols; col++)
				{
					his1_R[GetPixelValue(image1, row, col, R)] += 1;
					his1_G[GetPixelValue(image1, row, col, G)] += 1;
					his1_B[GetPixelValue(image1, row, col, B)] += 1;
				}

			for (int i = 0; i < 256; i++)
			{
				his1_R[i] /= (1.0 * image1.rows * image1.cols);
				his1_G[i] /= (1.0 * image1.rows * image1.cols);
				his1_B[i] /= (1.0 * image1.rows * image1.cols);
			}
			//Tính Histogram của ảnh 2
			for (int row = 0; row < image2.rows; row++)
				for (int col = 0; col < image2.cols; col++)
				{
					his2_R[GetPixelValue(image2, row, col, R)] += 1;
					his2_G[GetPixelValue(image2, row, col, G)] += 1;
					his2_B[GetPixelValue(image2, row, col, B)] += 1;
				}

			for (int i = 0; i < 256; i++)
			{
				his2_R[i] /= (1.0 * image1.rows * image1.cols);
				his2_G[i] /= (1.0 * image1.rows * image1.cols);
				his2_B[i] /= (1.0 * image1.rows * image1.cols);
			}
			//So Sánh hai histogram 
			// Công thức sum(abs(his1[i] - his2[i]))
			float percent_diff = 0.0;
			for (int i = 0; i < 256; i++)
			{
				float r_diff = 0.0;
				if (his1_R[i] != 0 || his2_R[i]!=0)
					r_diff = (his1_R[i] - his2_R[i])*(his1_R[i] - his2_R[i]) / (his1_R[i] + his2_R[i]);

				float g_diff = 0.0;
				if (his1_G[i] != 0 || his2_G[i] != 0)
					g_diff = (his1_G[i] - his2_G[i])*(his1_G[i] - his2_G[i]) / (his1_G[i] + his2_G[i]);

				float b_diff = 0;
				if (his1_B[i] != 0 || his2_B[i] != 0)
					b_diff = (his1_B[i] - his2_B[i])*(his1_B[i] - his2_B[i]) / (his1_B[i] + his2_B[i]);
				percent_diff = percent_diff + r_diff + g_diff + b_diff;
			}

			return sqrtf(percent_diff);
		}
		return 0;
	}
	
	
	ColorTransformer() 
	{

	}

	~ColorTransformer() 
	{

	}

private:

	/*
	Hàm vẽ lược đồ màu cho 1 kênh màu của ảnh
	Tham so :
		hist : mảng histogram của kênh màu
		color: màu của histogram
		histImage : ảnh histogram được vẽ
		window: tọa độ cửa sổ của để vẽ lên histImage
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int DrawHistogram_1C(const uint* hist, cv::Scalar color, cv::Mat& histImage, int window[4])
	{
		using namespace cv;

		// Tìm giá trị max của histogram
		int maxVal = 0;
		for (int i = 0; i < 256; ++i)
			if (maxVal < hist[i])
				maxVal = hist[i];

		// cân chỉnh lại histogram cho vừa với chiều cao window
		double scaleVal = (window[2] - window[0] + 1) / (double)maxVal;
		int scaled_hist[256];
		for (int i = 0; i < 256; ++i)
			scaled_hist[i] = (int)(hist[i] * scaleVal);

		// Vẽ histogram
		for (int i = window[1]; i <= window[3]; ++i)
		{
			int val = (double)(i - window[1]) / (window[3] - window[1] + 1) * 256;
			cv::rectangle(histImage,
				Rect(i, window[2] - scaled_hist[val] + 1, 1, scaled_hist[val]),
				color,
				FILLED);
		}

		return 1;
	}
};

