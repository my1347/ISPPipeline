
#pragma once
#include "stdafx.h"

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <vector> 

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>//Demosaiing in OpenCv inside



using namespace std;

#ifndef ISP_H
#define ISP_H

class ISP
{
public:
	//Declare the Default constructor
	ISP();
	//Declare the desconstructor
	~ISP();

	//Declare the Overload constructor
	ISP(uint16_t nInputHeight, uint16_t nInputWidth, uint8_t *pInput1ChannelBuffer);

	//Get Image3ChannelBuffer of ISP Class 
	void ISP::GetBuffer3Channal(uint8_t* pBuffer);

	//Demosaic (uint16*1 Chanal to uint16 *3 Chanal)
	void Demosaic();

	//ImageShow
	void ImageShow(const char *sWindowName);
	
	//WhiteBalance (uint16*3 Chanal to uint16 *3 Chanal)
	void WhiteBalance();

	//Normalized(uint16*1 Chanal to uint16 *1 Chanal)
	void Normalize(void *pInputRawBuffer, void *pOutputWBBuffer);

	//Brightness
	void Brightness(double dBrightnessParam);

	//Gamma
	void Gamma( double dGamma);

	//DEPRICATIED for now
	//Lumination Correction in Lab Color-Space(CLAHE (Contrast Limited Adaptive Histogram Equalization) algorithm)
	void CLAHE(void *pInputRawBuffer, void *pOutputWBBuffer);



	

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

	//Memory buffer pointer of 1 Channel image in BYTE(H*W*2*1)
	uint8_t * m_pBuffer1Channel;

	//Memory buffer pointer of 3 Channel image in BYTE(H*W*2*3)
	uint8_t * m_pBuffer3Channel;
	
};

#endif