#include "stdafx.h"

#include "ISP.h"
ISP::ISP()
{
	m_nHeight = 0;
	m_nWidth = 0;
	m_dPackedRawLength = 0;
	m_nImage1ChannelLength = 0;
	m_nImage3ChannelLength = 0;
	m_pBuffer1Channel = NULL;
	m_pBuffer3Channel = NULL;
		
}

ISP::~ISP()
{
	
	delete[] m_pBuffer1Channel;
	delete[] m_pBuffer3Channel;
}

ISP::ISP(uint16_t nInputHeight, uint16_t nInputWidth,uint8_t *pInput1ChannelBuffer) 
{
	m_nHeight = nInputHeight;
	m_nWidth = nInputWidth;
	m_dPackedRawLength = nInputWidth * nInputHeight*1.5;
	m_nImage1ChannelLength = nInputWidth * nInputHeight * 2;
	m_nImage3ChannelLength = nInputWidth * nInputHeight * 2 * 3;
	m_pBuffer1Channel = new uint8_t[m_nImage1ChannelLength];
	m_pBuffer3Channel = new uint8_t[m_nImage3ChannelLength];
	memcpy(m_pBuffer1Channel, pInput1ChannelBuffer, m_nImage1ChannelLength);
	
}

void ISP::GetBuffer3Channal(uint8_t* pBuffer)
{
	memcpy(pBuffer, m_pBuffer3Channel, m_nImage3ChannelLength);
}

void ISP::Demosaic()
{
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16U, m_pBuffer1Channel);
	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16UC3);
	cv::demosaicing(MatIn, MatOut, CV_BayerRG2RGB, 3);
	memcpy(m_pBuffer3Channel, MatOut.data,m_nImage3ChannelLength );//12579840*2*3(2Byte for 16bit,and 3 Chanel)


	////Debug show
	//cv::Mat covertedDemosaiedMatRealRaw;
	//MatOut.convertTo(covertedDemosaiedMatRealRaw, CV_8U);
	//cv::namedWindow("incovertedDemosaiedMatRealRaw", cv::WINDOW_NORMAL);
	//cv::imshow("incovertedDemosaiedMatRealRaw", covertedDemosaiedMatRealRaw);
	//cv::resizeWindow("incovertedDemosaiedMatRealRaw", int(nWidth / 6), int(nHeight / 6));
}

void ISP::ImageShow( const char *sWindowName)
{
	cv::Mat InputMat(int(m_nHeight),int(m_nWidth),CV_16UC3, m_pBuffer3Channel);
	cv::Mat covertedInputMat;
	InputMat.convertTo(covertedInputMat, CV_8U);
	cv::namedWindow(sWindowName, cv::WINDOW_NORMAL);
	cv::imshow(sWindowName, covertedInputMat);
	cv::resizeWindow(sWindowName, int(m_nWidth / 6), int(m_nHeight / 6));
}

void ISP::WhiteBalance() 
{
	//3 Chanal Buffer to Mat Obj
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16UC3, m_pBuffer3Channel);
	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16UC3);
	
	//WB Processing,With Stupid Grey World
	vector<cv::Mat> imageRGB;
	cv::split(MatIn, imageRGB);
		//InputImage RGB Chanel Mean Value
	double R, G, B;
	B = mean(imageRGB[0])[0];
	G = mean(imageRGB[1])[0];
	R = mean(imageRGB[2])[0];
		//RGB Gainer
	double GR, GG, GB;
	GB = (R + G + B) / (3 * B);
	GG = (R + G + B) / (3 * G);
	GR = (R + G + B) / (3 * R);
		//Adjust the RGB chanel with specify Gainer
	imageRGB[0] = imageRGB[0] * GB;
	imageRGB[1] = imageRGB[1] * GG;
	imageRGB[2] = imageRGB[2] * GR;
		//RGB Chanel Merge
	merge(imageRGB, MatOut);



	//3 Chaal Mat Obj to 3 Channal Buffer
	memcpy(m_pBuffer3Channel, MatOut.data, m_nImage3ChannelLength);
}

void ISP::Normalize(void *pInputRawBuffer, void *pOutputWBBuffer)
{
	//1 Chanal Buffer to Mat Obj
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16U, pInputRawBuffer);
	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16U);

	//Normalizing processing
	cv::normalize(MatIn,MatOut,0,255,cv::NORM_MINMAX);

	//1 Chaal Mat Obj to 3 Channal Buffer
	memcpy(pOutputWBBuffer, MatOut.data, m_nHeight*m_nWidth * 2);
}

void ISP::Gamma(double dGamma)
{
	//3 Chanal Buffer to Mat Obj
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16UC3, m_pBuffer3Channel);
	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16UC3);

	//Gamma
	

	cv::Mat Temp32FMat;
	cv::Mat Temp32FMatCoverted;
	cv::Mat Temp32FMatCovertedBackTo16;
	double dInverseGamma = (1/ dGamma);
	MatIn.convertTo(Temp32FMat, CV_32F);
	cv::pow(Temp32FMat, dInverseGamma, Temp32FMatCoverted);
	cv::normalize(Temp32FMatCoverted, Temp32FMatCovertedBackTo16, 0, 255, cv::NORM_MINMAX);
	Temp32FMatCovertedBackTo16.convertTo(MatOut, CV_16UC3);
	


	//3 Chaal Mat Obj to 3 Channal Buffer
	memcpy(m_pBuffer3Channel, MatOut.data, m_nImage3ChannelLength);
}

//void ISP::CLAHE(void *pInputRawBuffer, void *pOutputWBBuffer)
//{
//	//3 Chanal Buffer to Mat Obj
//	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16UC3, pInputRawBuffer);
//	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16UC3);
//
//	//Contrast Limited Adaptive Histogram Equalization
//		// READ RGB color image and convert it to Lab
//	cv::Mat lab_image;
//	cv::cvtColor(MatIn, lab_image, CV_RGB2Lab);
//
//		// Extract the L channel
//	std::vector<cv::Mat> lab_planes(3);
//	cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]
//
//		// apply the CLAHE algorithm to the L channel
//	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//	clahe->setClipLimit(4);
//	cv::Mat dst;
//	clahe->apply(lab_planes[0], dst);
//
//		// Merge the the color planes back into an Lab image
//	dst.copyTo(lab_planes[0]);
//	cv::merge(lab_planes, lab_image);
//
//		// convert back to RGB
//
//	cv::cvtColor(lab_image, MatOut, CV_Lab2RGB,3);
//
//	//3 Chanl Mat Obj to 3 Channal Buffer
//	memcpy(pOutputWBBuffer, MatOut.data, m_nHeight*m_nWidth * 2 * 3);
//
//
//}

void ISP::Brightness( double dBrightnessParam)
{
	//3 Chanal Buffer to Mat Obj
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16UC3, m_pBuffer3Channel);
	cv::Mat MatOut(m_nHeight, m_nWidth, CV_16UC3);

	//Brightness Processing
	cv::Mat MatGray;
	
	double dGrayScale;
	cv::cvtColor(MatIn, MatGray, cv::COLOR_RGB2GRAY);
	dGrayScale = dBrightnessParam*0.25*cv::mean(MatGray)[0];
	MatOut = MatIn*dGrayScale;
	//MatOut = min(255,MatIn*dGrayScale);
	
	//3 Chanl Mat Obj to 3 Channal Buffer
	memcpy(m_pBuffer3Channel, MatOut.data, m_nImage3ChannelLength);
}
