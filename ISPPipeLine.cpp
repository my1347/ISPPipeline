// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>//Demosaiing in OpenCv inside
#include <opencv2\xphoto.hpp>

#include "RawDataIO.h"
#include "ISP.h"

#define RAWFILE_NAME ("a.MTRaw")
#define RAWFILE_OFFSET (uint16_t(61440))
#define RAWIMAGE_W (uint16_t(4160))
#define RAWIMAGE_H (uint16_t(3024))
#define OUTPUTIMAGE_W (uint16_t(3968))
#define OUTPUTIMAGE_H (uint16_t(2976))
#define OUTPUTIMAGE_OFFSET_X (uint16_t(24))
#define OUTPUTIMAGE_OFFSET_Y (uint16_t(36))
#define OUTPUTFILE_NAME ("a.bmp")


using namespace std;

int main(void)
{
	//Temp Storage of RealRaw From RawDatIO object to transfer to ISP object
		uint8_t* pBuffer1Channal = new uint8_t[RAWIMAGE_H * RAWIMAGE_W * 2];
		uint8_t* pBuffer3Channal = new uint8_t[RAWIMAGE_H * RAWIMAGE_W * 2*3];
	
	//Create RawDataIO Object
	//RawDataIO NightRaw(RAWIMAGE_H,RAWIMAGE_W);
	RawDataIO *pNightRaw = new RawDataIO(RAWIMAGE_H, RAWIMAGE_W);

	//RawDataIO::Read Original Raw Buffer 
	//NightRaw.ReadRaw( RAWFILE_NAME, RAWFILE_OFFSET);
	pNightRaw->ReadRaw(RAWFILE_NAME, RAWFILE_OFFSET);

	//RawDataIO::Unpack Raw Buffer from 8bit in 3Byte to 12bit
	//NightRaw.UnpackRaw();
	pNightRaw->UnpackRaw();
	
	//GetBuffer1Channal in RawDataIO Private Member For the UnpakcedRealRaw	
	//NightRaw.GetBuffer1Channal(pBuffer1Channal);
	pNightRaw->GetBuffer1Channal(pBuffer1Channal);

	//Create ISP Object with Height and width
	//ISP NightRawISP(RAWIMAGE_H,RAWIMAGE_W, pBuffer1Channal);
	ISP *pNightRawISP = new ISP(RAWIMAGE_H, RAWIMAGE_W, pBuffer1Channal);

	//IPS::Demosaic Processing
	/*NightRawISP.Demosaic();
	NightRawISP.ImageShow( "Demosaic");*/
	pNightRawISP->Demosaic();
	pNightRawISP->ImageShow("Demosaic");

	//ISP::WhiteBlance Gaining
	/*NightRawISP.WhiteBalance();
	NightRawISP.ImageShow("WhiteBalance");*/
	pNightRawISP->WhiteBalance();
	pNightRawISP->ImageShow("WhiteBalance");

	//ISP::Brightness
	/*NightRawISP.Brightness(1);
	NightRawISP.ImageShow("Brightness");*/
	pNightRawISP->Brightness(1);
	pNightRawISP->ImageShow("Brightness");

	//ISP::Gamma
	/*NightRawISP.Gamma(2.2);
	NightRawISP.ImageShow("Gamma");*/
	pNightRawISP->Gamma(2.2);


	//ISP->RawDato
	/*NightRawISP.GetBuffer3Channal(pBuffer3Channal);*/
	pNightRawISP->GetBuffer3Channal(pBuffer3Channal);


	//RawDataIO::ImageCrop.Crop !AND! WRITE result to Bitmap BMP file
	//NightRaw.ImageCrop(pBuffer3Channal, pBuffer3Channal, OUTPUTIMAGE_OFFSET_X, OUTPUTIMAGE_OFFSET_Y, OUTPUTIMAGE_H, OUTPUTIMAGE_W, OUTPUTFILE_NAME);
	pNightRaw->ImageCrop(pBuffer3Channal, pBuffer3Channal, OUTPUTIMAGE_OFFSET_X, OUTPUTIMAGE_OFFSET_Y, OUTPUTIMAGE_H, OUTPUTIMAGE_W, OUTPUTFILE_NAME);


	//Delete Temp buffer For transfering buffer between different object
		delete[] pBuffer1Channal;
		delete[] pBuffer3Channal;
	
	//Delete the Objects
		delete pNightRaw;
		delete pNightRawISP;

	cv::waitKey(0);

	

}

