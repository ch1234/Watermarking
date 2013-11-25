		/*  VQR_16.C  */      /*ROUNDING*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
//#define EXIT_FAILURE 0

#define WIDTH    512     /* width of image  */
#define HEIGHT   512     /* height of image */

/****************************************************/
/* Edited version of provided VQ file. No mean in   */
/* this either. This encodes for a Codebook of size */
/* 16 and also writes the Codevector Indices out    */
/* to a file to allow them to be operated on to     */
/* calculate the polarities for embedding the       */
/* robust watermarks.                               */
/****************************************************/

typedef struct VECTOR{short int p[16];}vector;

vector *pt_cb;
FILE *f_in,*f_out, *f_out_indices,*f_cb;
int pels, codevectors;
short int width,height,limit,temp_var;
unsigned char *pt_in,*pt_out, output_indices[(HEIGHT/4)][(WIDTH/4)];

void open_files_VQ_s1_16();
void get_room_VQ_s1_16(void);
void encode_VQ_s1_16(void);

void VQ_Stage1NoMeanSize16(void)
{
	int i,j,k,l,m,w,h,n_l,n_b,n_r,temp,dmin,dist,index;
	short int round,temp1,pel[16];
	
	int a;
    
	pels=WIDTH*HEIGHT;
	codevectors = (WIDTH/4)*(HEIGHT/4);
	open_files_VQ_s1_16();

	// When performing VQ to extract watermarks after JPEG compression
	// Comment this line out because I don't store header info with the JPEGs
	fread((void *)&width,sizeof(width),(size_t)1,f_in);
	fread((void *)&height,sizeof(height),(size_t)1,f_in);
	
	if(width!=WIDTH||height!=HEIGHT){
		printf("Input is a %d * %d image.Abort.",width,height);
		getchar();
		exit(EXIT_FAILURE);
	}
	
	get_room_VQ_s1_16();
	
	fread((void *)pt_in,sizeof(char),(size_t)pels,f_in);
	fread((void *)pt_cb,sizeof(short int),(unsigned)256,f_cb);
	
	w=WIDTH/4;
	h=HEIGHT/4;
	n_l=WIDTH-4;
	n_b=(4*WIDTH)-4;
	n_r=3*WIDTH;
	
	round=16;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			m=0;
			
			for(k=0;k<4;k++){
				for(l=0;l<4;l++){
					pel[m]=(short int)*pt_in;
					m++;
					pt_in++;
				}
				pt_in+=n_l;
			}
			pt_in-=n_b;
		
			dmin=1500000L;
			for(l=0;l<16;l++){
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
			
			// This line was added here to output the Codevector Indices
			output_indices[i][j] = (unsigned char)index;
			
			for(k=0;k<4;k++){
				for(l=0;l<4;l++){
					limit=pt_cb[index].p[m];
					if(limit<0)limit=0;
					if(limit>255)limit=255;
					*pt_out=(unsigned char)limit;
					m++;
					pt_out++;
				}
				pt_out+=n_l;
			}
			pt_out-=n_b;
		}
		pt_in+=n_r;
		pt_out+=n_r;
	}
	pt_in-=pels;
	pt_out-=pels;

	// Writing codevectors to an output file. No header information needed.
	fwrite((void *)output_indices,sizeof(char),(size_t)codevectors,f_out_indices);
	width = WIDTH;
	height = HEIGHT;
	fwrite((void *)&width,sizeof(width),(size_t)1,f_out);
	fwrite((void *)&height,sizeof(height),(size_t)1,f_out);
	fwrite((void *)pt_out,sizeof(char),(size_t)pels,f_out);
	fclose(f_in);
	fclose(f_out);
	fclose(f_cb);
	free((void *)pt_in);
	free((void *)pt_out);
	free((void *)pt_cb);
	printf("Image vector quantised & codevectors generated.\n");
	//getchar();
}


void get_room_VQ_s1_16(void)
{
	pt_in=(unsigned char *)calloc((size_t)pels,sizeof(char));
	if(pt_in==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	pt_out=(unsigned char *)calloc((size_t)pels,sizeof(char));
	if(pt_out==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	pt_cb=(vector *)calloc((unsigned)256,sizeof(short int));
	if(pt_cb==NULL){
		printf("Out of memory.Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}

void open_files_VQ_s1_16()
{	
	// Choose locations of files here....
	if((f_in=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Watermarked Lena QF 100.Y","rb"))==NULL){
		printf("Cannot open file LENA_Y. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	if((f_out=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 1\\VQ_L_ENA_CB_16.Y","wb"))==NULL){
		printf("Cannot open file VQ_L_ENA. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_out_indices=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 1\\Codevectors.Y","wb"))==NULL){
		printf("Cannot open file VQ_L_ENA indices. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_cb=fopen("Codebooks\\cb16_16_stage1","rb"))==NULL){
		printf("Cannot open file codebook. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}













