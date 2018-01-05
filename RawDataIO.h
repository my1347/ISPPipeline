#pragma once
#include "stdafx.h"

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>

using namespace std;

#ifndef RawDataIO_H
#define RawDataIO_H



class RawDataIO
{
public:
	//Declare the constructor
	RawDataIO();

	//Declare the Overload Constructor
	RawDataIO(uint16_t, uint16_t);

	//Declare the desconstructor
	~RawDataIO();

	//GetBuffer
	void RawDataIO::GetBuffer1Channal(uint8_t* pBuffer);

	//Read raw file in Offset position with length of BYTE related to image Height and Width
	void ReadRaw(const char *pRawFileName, uint16_t nOffSet);

	//UnpackRaw from 8bit 3BYTE coded to 12bit,PackedRawLength is 12/8 larger than out put array
	void UnpackRaw();

	//Crop the RawSize Image
	void ImageCrop(void *pInputRawBuffer, void *pOutputWBBuffer, uint16_t nCropOffSetX, uint16_t nCropOffSetY, uint16_t nCropHeight, uint16_t nCropWidth, const char *pRawFileName);

	//Write Raw to BitMap
	/*void WriteBitmap(void *pInputRawBuffer, const char *pRawFileName, uint16_t nCropHeight, uint16_t nCropWidth);*/

private:
	uint16_t m_nHeight;
	uint16_t m_nWidth;

	//Packed Raw Legnth in BYTE,it is 12/8 larger than actual pixles.
	//in BYTE(W*H*12/8)
	double m_dPackedRawLength;

	//16bit 1 Channel in BYTE(W*H*2)
	double m_dRealRawLength;

	//1 Channel Length of 16bit Image in BYTE(H*W*2*1)
	double m_nImage1ChannelLength;

	//3 Channel Length of 16bit Image in BYTE(H*W*2*3)
	double m_nImage3ChannelLength;

	//Packed Raw buffer
	void * m_pPackedRawBuffer1Channel;

	//Unpacked Raw buffer
	void * m_pUnpakcedRawBuffer1Channel;

	//Memory buffer pointer of 1 Channel image in BYTE(H*W*2*1)
	uint8_t * m_pBuffer1Channel;

	//Memory buffer pointer of 3 Channel image in BYTE(H*W*2*3)
	uint8_t * m_pBuffer3Channel;
};

#endif