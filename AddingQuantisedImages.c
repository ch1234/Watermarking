#include<stdio.h>
#include<stdlib.h>		/* Memory Model: COMPACT */
#include<string.h>
#include<tchar.h>
#include "stdafx.h"

#define WIDTH    512	/* width of image */
#define HEIGHT   512    /* height of image */

/****************************************************/
/* This file adds the vq images generated after     */
/* each stage and adds them together to obtain a    */
/* more refined reconstruction image				*/
/****************************************************/


FILE			*f_picVQ1,*f_picVQ2, *f_pic_final;
int				pels;
short int		VQStage2_pic[HEIGHT][WIDTH],temp[HEIGHT][WIDTH];
unsigned char	VQStage1_pic[HEIGHT][WIDTH],Final_pic[HEIGHT][WIDTH];				

void AddingQuantisedImages(void)
{
	int row,col;
	short int width,height;
	
	static char *image1 = "VQ Stage 1 Images\\VQ_A_IRPLAN_CB_16.Y";
	static char *image2 = "VQ Stage 2 Images\\Unwatermarked\\VQ_ERROR_A_IRPLAN.Y";
	static char *image3 = "VQ Stage 2 Images\\Unwatermarked\\RECON_A_IRPLAN.Y";

	pels = WIDTH*HEIGHT;

	width  = WIDTH;
	height = HEIGHT;

	// Open files...

	f_picVQ1=fopen(image1,"rb");
	fread((void*)&width,sizeof(width),(size_t)1,f_picVQ1);
	fread((void*)&height,sizeof(height),(size_t)1,f_picVQ1);
	fread((void*)VQStage1_pic,sizeof(unsigned char),(size_t)pels,f_picVQ1);
	
	f_picVQ2=fopen(image2,"rb");
	fread((void*)&width,sizeof(width),(size_t)1,f_picVQ2);
	fread((void*)&height,sizeof(height),(size_t)1,f_picVQ2);
	fread((void*)VQStage2_pic,sizeof(short int),(size_t)pels,f_picVQ2);
	
	f_pic_final=fopen(image3,"wb");
		
	for(row=0;row<height;row++){											
		for(col=0;col<width;col++){
			temp[row][col] = (short int)VQStage1_pic[row][col] + VQStage2_pic[row][col];
				
			if (temp[row][col]>255)						// In case pixels >255 or <0.
					temp[row][col] = 255;
			if (temp[row][col]<0)
					temp[row][col] = 0;

			
			Final_pic[row][col] = (unsigned char)temp[row][col];
			}
	}

	fwrite((void*)&width,sizeof(width),(size_t)1,f_pic_final);				// Writing header info to file
	fwrite((void*)&height,sizeof(height),(size_t)1,f_pic_final);
	fwrite((void*)Final_pic,sizeof(unsigned char),(size_t)pels,f_pic_final);		// Writing pixels to file
	fclose(f_picVQ1);
	fclose(f_picVQ2);
	fclose(f_pic_final);
	printf("\nFinal 2-stage VQ'ed image generated.\n");
	//getchar();
}
