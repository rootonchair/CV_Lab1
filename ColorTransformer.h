#pragma once
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

