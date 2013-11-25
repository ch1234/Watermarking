#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"

//#define EXIT_FAILURE 0

/****************************************************/
/* This file calculates the normalised cross        */
/* correlation between the embedded and extracted   */
/* watermarks to provide a numerical value on how   */
/* similar they are. 1.0 = identical.               */
/****************************************************/

#define WIDTH			128     /* width of image  */
#define HEIGHT			128     /* height of image */

FILE *f_inRW1,*f_inERW1,*f_inRW2,*f_inERW2,*f_inRW3,*f_inERW3, *f_inSFW, *f_inESFW;	// RW = Robust Watermark, EW = Extracted Watermark, SF = Semi-Fragile
int pels,row,col;
short int width,height,w,h;

double CovR1,VarR1,CovR2,VarR2,CovR3,VarR3,NCR1,NCR2,NCR3; 
double CovSF,VarSF,NCSF;								// Fragile values

short int WatermarkR1[WIDTH][HEIGHT], ExtractedWatermarkR1[WIDTH][HEIGHT];
short int WatermarkR2[WIDTH][HEIGHT], ExtractedWatermarkR2[WIDTH][HEIGHT];
short int WatermarkR3[WIDTH][HEIGHT], ExtractedWatermarkR3[WIDTH][HEIGHT];
short int WatermarkSF[WIDTH][HEIGHT], ExtractedWatermarkSF[WIDTH][HEIGHT];

void open_filesWatermarksRobust();
void open_filesWatermarksSF();

void NormalisedCrossCorrelation()
{
	double ans;
	pels=(WIDTH)*(HEIGHT);	// Number of pixels in the image
	w = WIDTH;
	h = HEIGHT;
	
	open_filesWatermarksRobust();

	fread((void *)WatermarkR1,sizeof(short int),(size_t)pels,f_inRW1); 
	fread((void *)ExtractedWatermarkR1,sizeof(short int),(size_t)pels,f_inERW1);

	fread((void *)WatermarkR2,sizeof(short int),(size_t)pels,f_inRW2); 
	fread((void *)ExtractedWatermarkR2,sizeof(short int),(size_t)pels,f_inERW2);

	fread((void *)WatermarkR3,sizeof(short int),(size_t)pels,f_inRW3); 
	fread((void *)ExtractedWatermarkR3,sizeof(short int),(size_t)pels,f_inERW3);

	open_filesWatermarksSF();

	fread((void *)WatermarkSF,sizeof(short int),(size_t)pels,f_inSFW); 
	fread((void *)ExtractedWatermarkSF,sizeof(short int),(size_t)pels,f_inESFW);

	CovR1 = 0;
	VarR1 = 0;

	CovR2 = 0;
	VarR2 = 0;

	CovR3 = 0;
	VarR3 = 0;
	
	CovSF = 0;
	VarSF = 0;

	// Calculate NC numerator and denominator
	for (row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			CovR1 += (WatermarkR1[row][col])*(ExtractedWatermarkR1[row][col]);
			VarR1 += (WatermarkR1[row][col])*(WatermarkR1[row][col]);

			CovR2 += (WatermarkR2[row][col])*(ExtractedWatermarkR2[row][col]);
			VarR2 += (WatermarkR2[row][col])*(WatermarkR2[row][col]);

			CovR3 += (WatermarkR3[row][col])*(ExtractedWatermarkR3[row][col]);
			VarR3 += (WatermarkR3[row][col])*(WatermarkR3[row][col]);

			CovSF += (WatermarkSF[row][col])*(ExtractedWatermarkSF[row][col]);
			VarSF += (WatermarkSF[row][col])*(WatermarkSF[row][col]);
		}
	}

	NCR1 = CovR1/VarR1;
	NCR2 = CovR2/VarR2;
	NCR3 = CovR3/VarR3;
	NCSF = CovSF/VarSF;
	
	fclose(f_inRW1);
	fclose(f_inERW1);
	fclose(f_inRW2);
	fclose(f_inERW2);
	fclose(f_inRW3);
	fclose(f_inERW3);
	
	fclose(f_inSFW);
	fclose(f_inESFW);

	printf("\nNormalised Cross Correlation Robust 1 = %f",NCR1);
	printf("\nNormalised Cross Correlation Robust 2 = %f",NCR2);
	printf("\nNormalised Cross Correlation Robust 3 = %f",NCR3);
	printf("\nNormalised Cross Correlation Semi-Frag 3 = %f",NCSF);
}

void open_filesWatermarksRobust()
{
	if((f_inRW1=fopen("Watermarks\\Watermark1.Y","rb"))==NULL){
		printf("Cannot open Watermark1. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inERW1=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Watermarks\\EstWatermark1.Y","rb"))==NULL){
		printf("Cannot open file EstWatermark1. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inRW2=fopen("Watermarks\\Watermark2.Y","rb"))==NULL){
		printf("Cannot open Watermark2. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inERW2=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Watermarks\\EstWatermark2.Y","rb"))==NULL){
		printf("Cannot open file EstWatermark2. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inRW3=fopen("Watermarks\\Watermark3.Y","rb"))==NULL){
		printf("Cannot open Watermark3. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inERW3=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Watermarks\\EstWatermark3.Y","rb"))==NULL){
		printf("Cannot open file EstWatermark3.Y. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}


void open_filesWatermarksSF()
{
	if((f_inSFW=fopen("Watermarks\\Fragile Watermark.Y","rb"))==NULL){
		printf("Cannot open Watermark SF. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inESFW=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Watermarks\\EstWatermarkSF.Y","rb"))==NULL){
		printf("Cannot open file EstWatermark SF. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}