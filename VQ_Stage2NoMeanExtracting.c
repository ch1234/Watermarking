		/*  VQR_256.C  */      /*ROUNDING*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
//#define EXIT_FAILURE 0

#define WIDTH    512     /* width of image  */
#define HEIGHT   512     /* height of image */

/****************************************************/
/* This file is for extracting a semi-fragile		*/
/* watermark in the second stage vq. It uses a      */
/* regular vq operation and writes out the  		*/
/* codevector indices to a file. g3b128 images here */
/* since we're dealing with short ints              */
/****************************************************/


typedef struct VECTOR{short int p[16];}vector;

vector *pt_cb;
FILE *f_in,*f_out, *f_out_g3b128, *f_out_indices, *f_out_indices_g3b128, *f_cb;

int pels;
short int width,height,out_data,temp_var;

short int *pt_in,*pt_out, *pt_out_g3b128, output_indices[(HEIGHT/4)][(WIDTH/4)];
short int output_indices_G3B128[(HEIGHT/4)][(WIDTH/4)];

void open_files_VQ_s2_ne();
void get_room_VQ_s2_ne(void);

void VQ_Stage2NoMeanExtracting(void)
{
	int i,j,k,l,m,w,h,n_l,n_b,n_r,temp,dmin,dist,index, check;
	short int round,temp1,pel[16], codevectors;
	
	int a;
    
	pels=WIDTH*HEIGHT;
	
	open_files_VQ_s2_ne();
	
	fread((void *)&width,sizeof(width),(size_t)1,f_in);
	fread((void *)&height,sizeof(height),(size_t)1,f_in);
	
	if(width!=WIDTH||height!=HEIGHT){
		printf("Input is a %d * %d image.Abort.",width,height);
		getchar();
		exit(EXIT_FAILURE);
	}
	
	get_room_VQ_s2_ne();
	
	fread((void *)pt_in,sizeof(short int),(size_t)pels,f_in);
	fread((void *)pt_cb,sizeof(short int),(unsigned)4096,f_cb);
	
	w=WIDTH/4;
	h=HEIGHT/4;
	n_l=WIDTH-4;
	n_b=(4*WIDTH)-4;
	n_r=3*WIDTH;
	
	round=16;

	// 128 x 128 array to generate CVs...
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			m=0;

			// Converts Blocks of 4x4 into a single Codevector Index...
			for(k=0;k<4;k++){
				for(l=0;l<4;l++){
					pel[m]=*pt_in;
					m++;
					pt_in++;
				}
				pt_in+=n_l;	// Next line in block
			}
			pt_in-=n_b;	// Move onto next block.

			dmin=1500000L;	// Setting initial distance sufficiently high...

			// Loops through Codevectors in Codebook....Encoding Sequence
			for(l=0;l<256;l++){
				dist=0;
				for(m=0;m<16;m++){
					temp=(int)pel[m]-(int)pt_cb[l].p[m];
					dist+=temp*temp;
				}
				if(dist<dmin){
					dmin=dist;
					index=l;
				}
			}
			m=0;

			output_indices[i][j] = (short int)index;	// Using these values we can extract the watermark.

			// Gain3Bias128 for visual purposes. Clipping to [0,255] so displayed properly in Matlab.
			output_indices_G3B128[i][j] = (output_indices[i][j])*3;
			output_indices_G3B128[i][j] = output_indices_G3B128[i][j] + 128;
			if (output_indices_G3B128[i][j]>255)
				output_indices_G3B128[i][j] = 255;
			if (output_indices_G3B128[i][j]<0)
				output_indices_G3B128[i][j] = 0;

			//VQ-1 Operation...
			for(k=0;k<4;k++){
				for(l=0;l<4;l++){
					out_data=pt_cb[index].p[m];
					*pt_out=out_data;
					*pt_out_g3b128 = (*pt_out)*(3);
					*pt_out_g3b128 = *pt_out_g3b128 + 128;
					if (*pt_out_g3b128>255)
						*pt_out_g3b128 = 255;
					if (*pt_out_g3b128<0)
						*pt_out_g3b128 = 0;
					m++;
					pt_out++;
					pt_out_g3b128++;
				}
				pt_out+=n_l;
				pt_out_g3b128+=n_l;
			}
			pt_out-=n_b;
			pt_out_g3b128-=n_b;
		}
		pt_in+=n_r;
		pt_out+=n_r;
		pt_out_g3b128+=n_r;
	}
	pt_in-=pels;
	pt_out-=pels;
	pt_out_g3b128-=pels;

	width = WIDTH;
	height = HEIGHT;
	codevectors = w*h;

	fwrite((void *)output_indices,sizeof(short int),(size_t)codevectors,f_out_indices);
	fwrite((void *)output_indices_G3B128,sizeof(short int),(size_t)codevectors,f_out_indices_g3b128);

	fwrite((void *)&width,sizeof(width),(size_t)1,f_out);
	fwrite((void *)&height,sizeof(height),(size_t)1,f_out);
	fwrite((void *)pt_out,sizeof(short int),(size_t)pels,f_out);

	fwrite((void *)&width,sizeof(width),(size_t)1,f_out_g3b128);
	fwrite((void *)&height,sizeof(height),(size_t)1,f_out_g3b128);
	fwrite((void *)pt_out_g3b128,sizeof(short int),(size_t)pels,f_out_g3b128);
	
	fclose(f_in);
	fclose(f_out);
	fclose(f_out_g3b128);
	fclose(f_out_indices);
	fclose(f_cb);
	free((void *)pt_in);
	free((void *)pt_out);
	free((void *)pt_out_g3b128);
	free((void *)pt_cb);
	printf("2nd stage error vectors quantised & codevectors generated.\n");
	//getchar();
}


void get_room_VQ_s2_ne(void)
{
	pt_in=(short int *)calloc((size_t)pels,sizeof(short int));
	if(pt_in==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	pt_out=(short int *)calloc((size_t)pels,sizeof(short int));
	if(pt_out==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	pt_out_g3b128=(short int *)calloc((size_t)pels,sizeof(short int));
	if(pt_out==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	pt_cb=(vector *)calloc((unsigned)4096,sizeof(short int));
	if(pt_cb==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}

void open_files_VQ_s2_ne()
{

	if((f_in=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\ERROR.Y","rb"))==NULL){
		printf("Cannot open file LENA_Y. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	
	if((f_cb=fopen("Codebooks\\cb256_16_stage2","rb"))==NULL){
		printf("Cannot open file codebook. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 2\\VQ_ERROR.Y","wb"))==NULL){
		printf("Cannot open file VQ_L_ENA. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out_g3b128=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 2\\VQ_ERROR_G3B128.Y","wb"))==NULL){
		printf("Cannot open file VQ_L_ENA. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out_indices=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 2\\ErrorVectorIndices.Y","wb"))==NULL){
		printf("Cannot open file ER_L_ENA indices. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out_indices_g3b128=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 2\\ErrorVectorIndices_G3B128.Y","wb"))==NULL){
		printf("Cannot open file ER_L_ENA indices. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}