		/*  VQR_256.C	 */      /*ROUNDING*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"

#define WIDTH    512     /* width of image  */
#define HEIGHT   512     /* height of image */

/****************************************************/
/* This file is for embedding a semi-fragile		*/
/* watermark in the second stage vq. It uses an     */
/* index constrained method to assign codevectors   */
/* based on the current value of the watermark bit. */
/* The 2nd stage embedding vq isn't concerned with  */
/* writing the indices out.							*/
/****************************************************/


typedef struct VECTOR{short int p[16];}vector;

vector *pt_cb;
FILE *f_in,*f_watermark,*f_out, *f_out_g3b128, *f_cb;	//g3b128 allows for correct viewing of output vq image because it's
														//so possibly negative values
int pels, codevectors;
short int width,height,out_data,temp_var;

short int *pt_in,*pt_out, *pt_out_g3b128, Watermark[(HEIGHT/4)][(WIDTH/4)]

void open_files_VQ_s2();
void get_room_VQ_s2(void);

void VQ_Stage2NoMeanWithEmbedding(void)
{
	int i,j,k,l,m,w,h,n_l,n_b,n_r,temp,dmin,dist,index, check;
	short int round,temp1,pel[16];
	
	int a;
    
	pels=WIDTH*HEIGHT;
	
	open_files_VQ_s2();
	
	fread((void *)&width,sizeof(width),(size_t)1,f_in);
	fread((void *)&height,sizeof(height),(size_t)1,f_in);
	
	if(width!=WIDTH||height!=HEIGHT){
		printf("Input is a %d * %d image.Abort.",width,height);
		getchar();
		exit(EXIT_FAILURE);
	}
	
	get_room_VQ_s2();
	
	fread((void *)pt_in,sizeof(short int),(size_t)pels,f_in);
	fread((void *)pt_cb,sizeof(short int),(unsigned)4096,f_cb);
	
	fread((void *)Watermark,sizeof(short int),(size_t)pels,f_watermark);

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

			// Loops through Codevectors in Codebook. (Encoding Operation)
			for(l=0;l<128;l++){
				dist=0;
				// This line here is the index-constrained vq. if the watemark bit is high then the sequence is odd
				// as 2*n + 1 is an odd number. So sets loop through the odd codvectors. If the bit is low; even codevectors.
				check = 2*l + Watermark[i][j];
				for(m=0;m<16;m++){
					temp=(int)pel[m]-(int)pt_cb[check].p[m];
					dist+=temp*temp;
				}
				if(dist<dmin){
					dmin=dist;
					index=check;
				}
			}

			m=0;

			//Decoding Operation...
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

	fwrite((void *)&width,sizeof(width),(size_t)1,f_out);
	fwrite((void *)&height,sizeof(height),(size_t)1,f_out);
	fwrite((void *)pt_out,sizeof(short int),(size_t)pels,f_out);

	fwrite((void *)&width,sizeof(width),(size_t)1,f_out_g3b128);
	fwrite((void *)&height,sizeof(height),(size_t)1,f_out_g3b128);
	fwrite((void *)pt_out_g3b128,sizeof(short int),(size_t)pels,f_out_g3b128);
	
	fclose(f_in);
	fclose(f_out);
	fclose(f_out_g3b128);
	fclose(f_cb);
	free((void *)pt_in);
	free((void *)pt_out);
	free((void *)pt_out_g3b128);
	free((void *)pt_cb);
	printf("2nd stage error vectors quantised & codevectors generated.\n");
	//getchar();
}


void get_room_VQ_s2(void)
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

void open_files_VQ_s2()
{

	if((f_in=fopen("Error Images\\Using a Codebook of 16 for Stage 1\\ERROR_L_ENA.Y","rb"))==NULL){
		printf("Cannot open file Error LENA_Y. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	
	if((f_watermark=fopen("Watermarks\\Fragile Watermark.Y","rb"))==NULL){
		printf("Cannot open file Fragilr Watermark. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	
	if((f_cb=fopen("Codebooks\\cb256_16_stage2","rb"))==NULL){
		printf("Cannot open file codebook. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out=fopen("VQ Stage 2 Images\\Watermarked\\VQ_ERROR_L_ENA.Y","wb"))==NULL){
		printf("Cannot open file VQ_L_ENA. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out_g3b128=fopen("VQ Stage 2 Images\\Watermarked\\VQ_ERROR_L_ENA_G3B128.Y","wb"))==NULL){
		printf("Cannot open file VQ_ER_L_ENA g3b128. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}