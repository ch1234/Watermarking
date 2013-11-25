#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"
//#define EXIT_FAILURE 0

#define WIDTH    128     /* width of codevectors array  */
#define HEIGHT   128     /* height of codevectors array */

/****************************************************/
/* This file reads in the codevector indices        */
/* from the vq stage 1 operation and calculates the */
/* means of the indices and their neighbouring      */
/* indices. Writes to file so polarities and        */
/* variances can be calculated later on.            */
/****************************************************/

FILE *f_in,*f_out;
int pels;
short int width,height,w,h;

unsigned char codevectors_input[HEIGHT][WIDTH], mean_codevectors[HEIGHT][WIDTH];
short int temp[HEIGHT][WIDTH];	// Temp short int for arithmetic 

void open_files_mean();

void calcBlockMeans()
{
	int row,col,m,n,i,j;

	pels=(WIDTH)*(HEIGHT);	// Number of pixels in the image
	w = WIDTH;
	h = HEIGHT;
	open_files_mean();
	
	fread((void *)codevectors_input,sizeof(unsigned char),(size_t)pels,f_in); // Reads pixels in and stores them in codevectors_input
	
	// Assign borders of mean output to original VQ values to stop array going out of scope when calculating the mean.
	for(row=0;row<(HEIGHT);row++){
		mean_codevectors[row][0] = codevectors_input[row][0];
		mean_codevectors[row][(WIDTH-1)] = codevectors_input[row][(WIDTH-1)];
	}
	
	for(col=0;col<(WIDTH);col++){
		mean_codevectors[0][col] = codevectors_input[0][col];	
		mean_codevectors[(HEIGHT-1)][col] = codevectors_input[(HEIGHT-1)][col];
	}

	// Calculate the sum of each block, start in 1 row and in 1 column...
	for(m=1;m<(HEIGHT-1);m++){
		for(n=1;n<(WIDTH-1);n++){	
			temp[m][n] = ((short int)codevectors_input[m-1][n-1] + (short int)codevectors_input[m-1][n] + (short int)codevectors_input[m-1][n+1] +
									(short int)codevectors_input[m][n-1] + (short int)codevectors_input[m][n] + (short int)codevectors_input[m][n+1] +
									(short int)codevectors_input[m+1][n-1] + (short int)codevectors_input[m+1][n] + (short int)codevectors_input[m+1][n+1]);
		}
		n=1;	// Set back to the start of new row and repeat above...
	}

	for(i=1;i<(HEIGHT-1);i++){
		for(j=1;j<(WIDTH-1);j++){
			temp[i][j] = (temp[i][j])/9;						// Determine mean
			mean_codevectors[i][j] = (unsigned char)temp[i][j];	// Assign to unsigned char
		}
	}

	//No header information...
	fwrite((void *)mean_codevectors,sizeof(char),(size_t)pels,f_out);	// Writing pixels to file
	fclose(f_in);
	fclose(f_out);
	printf("Means calculated.\n");
	//getchar();
}

void open_files_mean()
{
	if((f_in=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\CodevectorIndicesForVQ_16_RECON_L_ENA_QF_30.Y","rb"))==NULL){
		printf("Cannot open codevectors file. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	if((f_out=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Means.Y","wb"))==NULL){
		printf("Cannot open file mean of codevectors. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}