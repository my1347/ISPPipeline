#include "stdafx.h"

#include "RawDataIO.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>//Demosaiing in OpenCv inside


RawDataIO::RawDataIO()
{
	m_nHeight = 0;
	m_nWidth = 0;
	m_dPackedRawLength = 0;
	m_nImage1ChannelLength = 0;
	m_nImage3ChannelLength = 0;
	m_pBuffer1Channel = NULL;
	m_pBuffer3Channel = NULL;
	m_pPackedRawBuffer1Channel = NULL;
	m_pUnpakcedRawBuffer1Channel = NULL;

}

RawDataIO::RawDataIO(uint16_t nInputHeight, uint16_t nInputWidth)
{
	m_nHeight = nInputHeight;
	m_nWidth = nInputWidth;
	m_dPackedRawLength = nInputWidth * nInputHeight*1.5;
	m_nImage1ChannelLength = nInputWidth * nInputHeight*2;
	m_nImage3ChannelLength = nInputWidth * nInputHeight * 2 * 3;
	m_pBuffer1Channel = new uint8_t[m_nImage1ChannelLength];
	m_pBuffer3Channel = new uint8_t[m_nImage3ChannelLength];
	m_pPackedRawBuffer1Channel = new uint8_t[m_dPackedRawLength];
	m_pUnpakcedRawBuffer1Channel = new uint8_t[m_nImage1ChannelLength];

}

RawDataIO::~RawDataIO()
{
	delete[] m_pPackedRawBuffer1Channel;
	delete[] m_pUnpakcedRawBuffer1Channel;
	delete[] m_pBuffer1Channel;
	delete[] m_pBuffer3Channel;
}

void RawDataIO::GetBuffer1Channal(uint8_t* pBuffer)
{
	memcpy(pBuffer, m_pBuffer1Channel, m_nImage1ChannelLength);
}


void RawDataIO::ReadRaw(const char *pRawFileName,uint16_t nOffSet)
{
	

	//Open the RawFile
	FILE *inputFile = NULL;
	inputFile = fopen(pRawFileName, "rb");

	// set position to beginning of RealRaw position
	fseek(inputFile, nOffSet, SEEK_SET);

	// read PackedRaw Length
	fread(m_pPackedRawBuffer1Channel, 1, m_dPackedRawLength, inputFile);
}

void RawDataIO::UnpackRaw()
{
	typedef char 	BYTE;
	typedef uint16_t 	adc16_t;
	const BYTE* in8 = static_cast<const BYTE*>(m_pPackedRawBuffer1Channel);
	const BYTE* stop = in8 + (m_nWidth*m_nHeight*3/2);

	adc16_t* out16 = static_cast<adc16_t*>(m_pUnpakcedRawBuffer1Channel);

	while (in8 != stop)
	{
		BYTE v1 = *in8++;
		BYTE v2 = *in8++;
		BYTE v3 = *in8++;

		*out16++ = ((v1 << 4) + ((v2 >> 4) & 0xf)) & 0xfff;
		*out16++ = (((v2 & 0xf) << 8) + (v3 & 0xff)) & 0xfff;
	}

	memcpy(m_pBuffer1Channel, m_pUnpakcedRawBuffer1Channel, m_nImage1ChannelLength);

}

void RawDataIO::ImageCrop(void *pInputRawBuffer, void *pOutputBuffer, uint16_t nCropOffSetX, uint16_t nCropOffSetY, uint16_t nCropHeight, uint16_t nCropWidth, const char *pRawFileName)
{
	//3 Chanal Buffer to Mat Obj
	cv::Mat MatIn(m_nHeight, m_nWidth, CV_16UC3, pInputRawBuffer);
	cv::Mat MatOut(nCropHeight, nCropWidth, CV_16UC3);

	//Cropping
	MatOut = MatIn(cv::Rect(nCropOffSetX, nCropOffSetY, nCropWidth, nCropHeight));//CropOffsetXY 24,36;CropHeight Width 2976 3968)

	//ImageWrite
	cv::imwrite(pRawFileName, MatOut/*,compression_params*/);

	//3 Chanl Mat Obj to 3 Channal Buffer
	memcpy(pOutputBuffer, MatOut.data, nCropHeight * nCropWidth * 2 * 3);

	//Imshow For debugging
	cv::Mat covertedInputMat;
	MatOut.convertTo(covertedInputMat, CV_8U);
	cv::namedWindow("Cropped", cv::WINDOW_NORMAL);
	cv::imshow("Cropped", covertedInputMat);
	cv::resizeWindow("Cropped", int(nCropWidth / 6), int(nCropHeight / 6));
}

//void RawDataIO::WriteBitmap(void *pInputRawBuffer, const char *pRawFileName, uint16_t nCropHeight, uint16_t nCropWidth)
//{
//	//3 Chanal Buffer to Mat Obj
//	cv::Mat MatIn(nCropHeight, nCropWidth,  CV_16UC3, pInputRawBuffer);
//
//	//MatIn.convertTo(covertedInputMat, CV_8U);
//
//
//	//vector<int> compression_params;
//	//compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//	//compression_params.push_back(9);
//	cv::imwrite(pRawFileName, MatIn/*,compression_params*/);
//
//
//
//
//	Imshow For debugging
//	cv::Mat covertedInputMat;
//	MatIn.convertTo(covertedInputMat, CV_8U);
//	cv::namedWindow("CroppedInForWrite", cv::WINDOW_NORMAL);
//	cv::imshow("CroppedInForWrite", covertedInputMat);
//	cv::resizeWindow("CroppedInForWrite", int(nCropWidth / 6), int(nCropHeight / 6));
//}