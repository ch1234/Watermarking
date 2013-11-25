		/* LBG.C */
		/* LBG algorithm: generates 256 4*4 codevectors */
		/* from a training set of 8192 4*4 vectors */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"

#define EXIT_FAILURE 0

/****************************************************/
/* Generates a codebook of size 256 for 2nd stage   */
/* vq operation based on the error vector TS.    	*/
/****************************************************/


typedef struct VECTOR{short int p[16];}vector;
typedef struct CLUSTER{int p[16];int f;}cluster;

vector *pt_cb, *pt_ts;
cluster cltr[256];
FILE *f_ts, *f_cb;

void generate_s2(void);

void LBG_GenerateCodebookForStageTwo(void)
{
	f_ts=fopen("Training Sets\\ts8192_no_mean_stage2","rb");
	if(f_ts==NULL){
		puts("File ts8192_no_mean_stage2 does not exist.");
		exit(EXIT_FAILURE);
	}
	f_cb=fopen("Codebooks\\cb256_16_stage2","wb");
	if(f_cb==NULL){
		puts("File cb256_16_stage2 does not exist.");
		exit(EXIT_FAILURE);
	}
	pt_ts=(vector *)calloc((size_t)131072UL,sizeof(short int));
	if(pt_ts==NULL){
		printf("Out of memory.Abort.\n");
		exit(EXIT_FAILURE);
	}
	pt_cb=(vector *)calloc((unsigned)4096,sizeof(short int));		// Check unsigned or short int???
	if(pt_cb==NULL){
		printf("Out of memory.Abort.\n");
		exit(EXIT_FAILURE);
	}
	fread((void *)pt_ts,sizeof(short int),(size_t)131072UL,f_ts);
	generate_s2();
	fwrite((void *)pt_cb,sizeof(short int),(size_t)4096,f_cb);
	fclose(f_cb);
	fclose(f_ts);
	free((void *)pt_cb);
	free((void *)pt_ts);
	printf("\nCodebook generated 2nd stage.\n");
}

void generate_s2(void)
{
	int i,j,k,l,index,dtr,dmin,temp,distortion,dist;
	float eps,dist1;

	dtr=400000000L;
	eps=0.01;			/* EPSILON */
	for(i=0;i<256;i++){
		j=i*32;
		for(k=0;k<16;k++){
			pt_cb[i].p[k]=pt_ts[j].p[k];
		}
	}
	A:;
	for(i=0;i<256;i++){
		for(j=0;j<16;j++){
			cltr[i].p[j]=0;
		}
		cltr[i].f=0;
	}
	distortion=0;
	for(i=0;i<8192;i++){
		dmin=1500000L;
		for(j=0;j<256;j++){
			dist=0;
			for(k=0;k<16;k++){
			temp=(int)pt_ts[i].p[k]-(int)pt_cb[j].p[k];
				dist=dist+(temp*temp);
			}
			if(dist<dmin){
				dmin=dist;
				index=j;
			}
		}
		for(l=0;l<16;l++){
			cltr[index].p[l]+=(int)pt_ts[i].p[l];
		}
		cltr[index].f++;
		distortion+=dmin;
	}
	for(i=0;i<256;i++){
		if(cltr[i].f==0){
			continue;
		}
		for(j=0;j<16;j++){
			temp=cltr[i].p[j]/cltr[i].f;
			pt_cb[i].p[j]=(short int)temp;
		}
	}
	dist1=(float)(dtr-distortion)/(float)distortion;
	if(dist1>eps){
		dtr=distortion;
		goto A;
	}
}


