#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"

//#define EXIT_FAILURE 0

#define WIDTH	 128     /* width of image  */
#define HEIGHT   128     /* height of image */

/****************************************************/
/* This file reads in the codevector indices        */
/* from the vq stage 1 operation and the mean       */
/* values of the codevector indices and uses these  */
/* to calculate the variance values                 */
/****************************************************/


FILE *f_in,*f_inmv,*f_outv;		// inmv = input mean values. outv = variance
int pels;
short int width,height,w,h,width1,height1;	

unsigned char codevectors_input[HEIGHT][WIDTH], mean_codevectors[HEIGHT][WIDTH]; 
short int SquareOfMean[HEIGHT][WIDTH], MeanSquared[HEIGHT][WIDTH], tempMeanSquared[HEIGHT][WIDTH], InputSquared[HEIGHT][WIDTH], Variance[HEIGHT][WIDTH];

void open_filesVariances();

void calcBlockVariances()
{
	short int a,b,c,d,e,f,g,h;	// Temporary variables
	int row,col,m,n,i,j;		// Making these ints instead of short int because they're counters and not pixel values. (Easier to remember)

	pels=(WIDTH)*(HEIGHT);		// Number of pixels in the image
	w = WIDTH;
	h = HEIGHT;
	open_filesVariances();
	
	
	fread((void *)codevectors_input,sizeof(unsigned char),(size_t)pels,f_in); // Reads pixels in and stores them in codevectors_input
	fread((void *)mean_codevectors,sizeof(unsigned char),(size_t)pels,f_inmv); // Read in mean from previous file.	
	
	// Squaring input mean values...
	for(row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			a = (short int)mean_codevectors[row][col];
			b = a*a;
			SquareOfMean[row][col] = b;
		}
	}

	// Assign borders of mean squared to original VQ values squared to stop array going out of scope when calculating the mean squared.
	// Use temp variables to calculate squares...
	for(row=0;row<(HEIGHT);row++){
		c = (short int)codevectors_input[row][0];
		d = c*c;
		MeanSquared[row][0] = d;
		
		c = codevectors_input[row][(WIDTH-1)];
		d = c*c;
		MeanSquared[row][(WIDTH-1)] = d;
	}
	
	for(col=0;col<WIDTH;col++){
		e = (short int)codevectors_input[0][col];
		f = e*e;
		MeanSquared[0][col] = f;

		e = (short int)codevectors_input[(HEIGHT-1)][col];
		f = e*e;
		MeanSquared[(HEIGHT-1)][col] = f;
	}

	// Squaring input pixels...
	for(row=0;row<HEIGHT;row++){
		for(col=0;col<WIDTH;col++){
			g = (short int)codevectors_input[row][col];
			h = g*g;
			InputSquared[row][col] = h;
		}
	}

	// Calculate the sum of each neighbourhood block and start in 1 row and in 1 column...
	for(m=1;m<(HEIGHT-1);m++){
		for(n=1;n<(WIDTH-1);n++){	
			tempMeanSquared[m][n] = (InputSquared[m-1][n-1] + InputSquared[m-1][n] + InputSquared[m-1][n+1] +
									InputSquared[m][n-1] + InputSquared[m][n] + InputSquared[m][n+1] +
									InputSquared[m+1][n-1] + InputSquared[m+1][n] + InputSquared[m+1][n+1]);
		}
		n=1;
	}

	for(i=1;i<(HEIGHT-1);i++){
		for(j=1;j<(WIDTH-1);j++){
			MeanSquared[i][j] = (tempMeanSquared[i][j])/9;		// Determine mean-squared
		}
	}
	
	// Calculating variance...
	for(i=0;i<(HEIGHT);i++){
		for(j=0;j<(WIDTH);j++){
			Variance[i][j] = MeanSquared[i][j] - SquareOfMean[i][j] ;	// Compute variance
			}
	}

	fwrite((void *)Variance,sizeof(short int),(size_t)pels,f_outv);	// Writing variances to file. Note: short int value

	fclose(f_in);
	fclose(f_inmv);
	fclose(f_outv);
	printf("Variances calculated.\n");
	//getchar();
}

void open_filesVariances()
{
	if((f_in=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\CodevectorIndicesForVQ_16_RECON_L_ENA_QF_30.Y","rb"))==NULL){
		printf("Cannot open file codevectors. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_inmv=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Means.Y","rb"))==NULL){
		printf("Cannot open mean codevectors. Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}

	if((f_outv=fopen("Jpeg Compression\\Stage 1 and 2\\Qf 30\\Stage 1\\Variances.Y","wb"))==NULL){
		printf("Cannot open file test_cmp.png .Abort.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
}