							/*  LOADTS.C  */   
							/*ROUNDING*/
#include<stdio.h>
#include<stdlib.h>		/* Memory Model: COMPACT */
#include<string.h>
#include <tchar.h>
#include "stdafx.h"

#define WIDTH    512	/* width of image */
#define HEIGHT   512    /* height of image */
#define START    2052   /* first vector is 2052 pels offset */
#define NXT_LI   508	/* next line */
#define NXT_BL	 2032   /* next block */

/****************************************************/
/* Similar to TS file for 1st stage, but this       */
/* generates a training set based on the error      */
/* vectors from the 1st stage vq. There is no mean  */
/* calculated here either.                          */
/****************************************************/


void LoadTSNoMeanStage2()
{
	int i,j,k,l,m,pel;
	short int *pt_ts,*ptr_ts,*pt_pic, width,height,w,b_m;

	FILE *f_pic,*f_ts;

	static char *image[8]= {"Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_A_IRPLAN.Y", "Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_B_AR.Y", "Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_B_OAT.Y",
							"Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_M_ANDRIL.Y", "Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_M_OFFETT.Y", "Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_P_EPPERS.Y",
							"Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_S_AILBOA.Y", "Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_T_IFFANY.Y"};

	w=WIDTH*16;
	pt_pic=(short int *)calloc((size_t)w,sizeof(short int));
	if(pt_pic==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	f_ts=fopen("Training Sets\\ts8192_no_mean_stage2","wb");
	if(f_ts==NULL){
		printf("Cannot open file ts8192m3.Abort.");
		exit(EXIT_FAILURE);
	}
	pt_ts=(short int *)calloc((size_t)16384,sizeof(short int));
	if(pt_ts==NULL){
		printf("Out of memory.Abort.");
		exit(EXIT_FAILURE);
	}
 
	ptr_ts=pt_ts;
	for(i=0;i<8;i++){
		f_pic=fopen(image[i],"rb");
		if(f_pic==NULL){
			printf("Cannot open file %s.Abort",image[i]);
			getchar();
			exit(EXIT_FAILURE);
		}
		fread((void *)&width,sizeof(width),(size_t)1,f_pic);
		fread((void *)&height,sizeof(height),(size_t)1,f_pic);
		if(width!=WIDTH||height!=HEIGHT){
			printf("%s is not a 512*512 image",image[i]);
			exit(EXIT_FAILURE);
		}
		for(j=0;j<32;j++){
			/* read 16 lines of the image */
			fread((void *)pt_pic,sizeof(short int),(size_t)w,f_pic);
			pel=START;
			for(k=0;k<32;k++){
				for(l=0;l<4;l++){
					for(m=0;m<4;m++){
						*ptr_ts=pt_pic[pel];
						ptr_ts++;
						pel++;
					}
					pel+=NXT_LI;
				}
				pel-=NXT_BL;
			}
		}
		ptr_ts=pt_ts;

	fwrite((void *)pt_ts,sizeof(short int),(size_t)16384,f_ts);
	}
	printf("Training set stage 2 completed");
	//getchar();
}