#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"

//#define EXIT_FAILURE 0

#define WIDTH			128     /* width of image  */
#define HEIGHT			128     /* height of image */
#define CODEBOOK_SIZE	16

/****************************************************/
/* Reads mean and variance values from prev         */
/* files and uses these to calculate the            */
/* polarities that will be used to embed the        */
/* secret keys                                      */
/****************************************************/


FILE *f_inm,*f_inv,*f_outP1, *f_outP2, *f_outP3;	// Read in mean and variance. Output polarities as 3 individual files.
int pels;
short int width,height,width1,height1,w,h;

unsigned char Mean_input[HEIGHT][WIDTH];
short int Variance_input[HEIGHT][WIDTH];
unsigned char Polarity1[HEIGHT][WIDTH], Polarity2[HEIGHT][WIDTH], Polarity3[HEIGHT][WIDTH]; 

void open_filesPolarities();

void calcPolarities()
{
	int row,col,m,n,i,j,a,b,c;
	int threshold = (CODEBOOK_SIZE)/2;

	pels=(WIDTH)*(HEIGHT);	// Number of pixels in the image
	w = WIDTH;
	h = HEIGHT;
	
	open_filesPolarities();

	fread((void *)Mean_input,sizeof(unsigned char),(size_t)pels,f_inm); // Reads in mean values and stores them in mean_input array
	fread((void *)Variance_input,sizeof(short int),(size_t)pels,f_inv);

	// Calculate Polarity 1...i.e. is mean>= threshold
	for (row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			if((short int)Mean_input[row][col]>=threshold)
				Polarity1[row][col]=1;
			else
				Polarity1[row][col]=0;
		}
	}

	// Calculate Polarity 2...i.e. is variance >= threshold
	for (row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			if(Variance_input[row][col]>=threshold)
				Polarity2[row][col]=1;
			else
				Polarity2[row][col]=0;
		}
	}

	// Calculate Polarity 3...i.e. mean xor variance
	for (row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			a = (short int)Polarity1[row][col];
			b = (short int)Polarity2[row][col];

			if(a != b)
				Polarity3[row][col] = 1;
			else 
				Polarity3[row][col] = 0;
		}
	}

	fwrite((void *)Polarity1,sizeof(char),(size_t)pels,f_outP1);	// Writing pixels to file
	
	fwrite((void *)Polarity2,sizeof(char),(size_t)pels,f_outP2);	// Writing pixels to file
	
	fwrite((void *)Polarity3,sizeof(char),(size_t)pels,f_outP3);	// Writing pixels to file
	
	fclose(f_inm);
	fclose(f_inv);
	fclose(f_outP1);
	fclose(f_outP2);
	fclose(f_outP3);
	printf("Polarities calculated.\n");
	//getchar();
}

void open_filesPolarities()
{
	if((f_inm=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Means.Y","rb"))==NULL){
		printf("Cannot open mean .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inv=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Variances.Y","rb"))==NULL){
		printf("Cannot open file variance .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_outP1=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Polarity1.Y","wb"))==NULL){
		printf("Cannot open file polarity1 .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_outP2=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Polarity2.Y","wb"))==NULL){
		printf("Cannot open file polarity2 .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_outP3=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Polarity3.Y","wb"))==NULL){
		printf("Cannot open file polarity3 .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}