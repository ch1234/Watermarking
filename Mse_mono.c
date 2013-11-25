						/*  MSE_MONO.C  */
#include<stdio.h>
#include<stdlib.h>		/* Memory Model: COMPACT */
#include<math.h>
#include"stdafx.h"

/****************************************************/
/* This file was provided at the start of the       */
/* project. It calculates the MSE and PSNR values   */
/****************************************************/

		/* width of image < 800 */
		/* height of image(a multiple of 8) */
		/* argv[1]=original image */
		/* argv[2]=coded image */
		/* tspe=total squared pixel error for decoded picture */
		/* mse=tspe/number of pixels */
		/* psnr=10log[(255*255)/mse] */

void mse_mono()
{
	unsigned char *pt_oi,*pt_ci,*ptr1,*ptr2;
	short int width,height,width1,height1,w,h;
	FILE *f_oi,*f_ci;
	int i,j;
	double pels,tspe,mse,psnr,temp;

	static char *image1 = "Test Images\\L_ENA.Y";
	static char *image2 = "VQ Attacks\\Stage 1 and 2\\Kohonen Self Organising Feature Map\\KSO_VQ_RECON_LENA.Y";

	if((f_oi=fopen(image1,"rb"))==NULL){
		printf("Cannot open file oi.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	if((f_ci=fopen(image2,"rb"))==NULL){
		printf("Cannot open file ci.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	fread((void *)&width,sizeof(width),(size_t)1,f_oi);    /* width=216;*/
	fread((void *)&height,sizeof(height),(size_t)1,f_oi);  /* height=216;*/
	fread((void *)&width1,sizeof(width1),(size_t)1,f_ci);  /* width1=216;*/
	fread((void *)&height1,sizeof(height1),(size_t)1,f_ci);/* height1=216;*/
	if(width!=width1||height!=height1){
		printf("Different image sizes.Abort.");
		getchar();
		exit(EXIT_FAILURE);
	}
	w=width<<3;
	h=height>>3;
	pt_oi=(unsigned char *)calloc((size_t)w,sizeof(char));
	if(pt_oi==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	pt_ci=(unsigned char *)calloc((size_t)w,sizeof(char));
	if(pt_ci==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	tspe=0;
	for(i=0;i<h;i++){
		fread((void *)pt_oi,sizeof(char),(size_t)w,f_oi);
		fread((void *)pt_ci,sizeof(char),(size_t)w,f_ci);
		ptr1=pt_oi;
		ptr2=pt_ci;
		for(j=0;j<w;j++){
			temp=(double)*ptr1-(double)*ptr2;
			tspe+=temp*temp;
			ptr1++;
			ptr2++;
		}
	}
	free((void *)pt_oi);
	free((void *)pt_ci);
	fclose(f_oi);
	fclose(f_ci);
	pels=(double)width*(double)height;
	mse=tspe/pels;
	psnr=10*log10((255.0*255.0)/mse);
	printf("\nOriginal monochrome image: %s \n",image1);
	printf("Vector quantized image: %s \n",image2);
	printf("Total squared pixel error=%e\n",tspe);
	printf("Mean squared error=%e\n",mse);
	printf("Peak signal to noise ratio=%edB\n\n",psnr);
	//getchar();
}