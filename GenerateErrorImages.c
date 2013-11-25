#include<stdio.h>
#include<stdlib.h>		/* Memory Model: COMPACT */
#include<string.h>
#include<tchar.h>
#include "stdafx.h"

#define WIDTH    512	/* width of image */
#define HEIGHT   512    /* height of image */

/****************************************************/
/* Generates error images by taking the difference  */
/* between  the input image  and the vq stage 1 o/p */
/* Error image is also output with gain of 3 and    */
/* a bias of 128 pixels to be able to display       */
/* correctly in Matlab.                             */
/****************************************************/

FILE *f_Original_Image,*f_VQ_Image, *f_pic_error, *f_pic_error_gb;
unsigned char Original_Image[HEIGHT][WIDTH], VQ_Image[HEIGHT][WIDTH];
short int Error_Image[HEIGHT][WIDTH], Error_Image_G3B128[HEIGHT][WIDTH]; //G3B128 = Gain of 3 and Bias 0f 128 pixels
int pels;

void open_files_ei();

void GenerateErrorImages()
{
	int i,j,k,l,m,pel,row,col;
	short int width,height,w;

	pels = (WIDTH)*(HEIGHT);
	width = WIDTH;
	height = HEIGHT;

	// Open files...
	open_files_ei();

	// Strip out header information and read in data...
	fread((void*)&width,sizeof(width),(size_t)1,f_Original_Image);		
	fread((void*)&height,sizeof(height),(size_t)1,f_Original_Image);
	fread((void*)Original_Image,sizeof(char),(size_t)pels,f_Original_Image);

	fread((void*)&width,sizeof(width),(size_t)1,f_VQ_Image);
	fread((void*)&height,sizeof(height),(size_t)1,f_VQ_Image);
	fread((void*)VQ_Image,sizeof(char),(size_t)pels,f_VQ_Image);

	for(row=0;row<HEIGHT;row++){											
		for(col=0;col<WIDTH;col++){
			Error_Image[row][col] = (short int)Original_Image[row][col]-(short int)VQ_Image[row][col];
			Error_Image_G3B128[row][col] = (short int)Original_Image[row][col]-(short int)VQ_Image[row][col];
			
			Error_Image_G3B128[row][col] = Error_Image_G3B128[row][col]*3;		// Gain of 3 to view easier. (For report)
			Error_Image_G3B128[row][col] = Error_Image_G3B128[row][col] + 128;	// Bias of +128 pixels;

			// In case pixels >255 or <0.
			if (Error_Image_G3B128[row][col]>255)						
				Error_Image_G3B128[row][col] = 255;
			if (Error_Image_G3B128[row][col]<0)
				Error_Image_G3B128[row][col] = 0;
		}
	}

	fwrite((void *)&width,sizeof(width),(size_t)1,f_pic_error);					// Writing header info to file
	fwrite((void *)&height,sizeof(height),(size_t)1,f_pic_error);
	fwrite((void *)Error_Image,sizeof(short int),(size_t)pels,f_pic_error);		// Writing pixels to file

	fwrite((void *)&width,sizeof(width),(size_t)1,f_pic_error_gb);				// Writing header info to file
	fwrite((void *)&height,sizeof(height),(size_t)1,f_pic_error_gb);
	fwrite((void *)Error_Image_G3B128,sizeof(short int),(size_t)pels,f_pic_error_gb);

	fclose(f_Original_Image);
	fclose(f_VQ_Image);
	fclose(f_pic_error);
	fclose(f_pic_error_gb);
	printf("Error images generated.");
	//getchar();
}

void open_files_ei()
{
	if((f_Original_Image=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Watermarked Lena QF 100.Y","rb"))==NULL){
		printf("Cannot open file L_ENA.Y .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_VQ_Image=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 1\\VQ_L_ENA_CB_16.Y","rb"))==NULL){
		printf("Cannot open file VQ_L_ENA.Y. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_pic_error=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\ERROR.Y","wb"))==NULL){
		printf("Cannot open file error image. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_pic_error_gb=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\ERROR_GB.Y","wb"))==NULL){
		printf("Cannot open file error image gb. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}
