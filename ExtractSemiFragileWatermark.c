#include<stdio.h>
#include<stdlib.h>		/* Memory Model: COMPACT */
#include<string.h>
#include<tchar.h>
#include "stdafx.h"

#define WIDTH    128	/* width of image */
#define HEIGHT   128    /* height of image */

/****************************************************/
/* Reads in the error codevector indices from       */
/* the 2nd stage vq and performs the modulo-2       */
/* operation on the indices. As stated before, if   */
/* the watermark bit is 1, then the codevector is   */
/* odd. So if the modulo-2 operation produces a     */
/* a remainder then the watermark bit is 1 and      */
/* vice-versa.                                      */
/****************************************************/

FILE			*f_pic_vq2_indices,*f_pic_ex_wmark;
int				pels;
short int		CodevectorIndices[HEIGHT][WIDTH],ExtractedWatermark[HEIGHT][WIDTH];

void ExtractSemiFragileWatermark(void)
{
	int row,col,check;
	short int width,height;
	
	static char *image1 = "Jpeg Compression\\Stage 1 and 2\\Qf 100\\Stage 2\\ErrorVectorIndices.Y";
	static char *image2 = "Jpeg Compression\\Stage 1 and 2\\Qf 100\\Watermarks\\EstWatermarkSF.Y";
	
	pels = WIDTH*HEIGHT;

	width  = WIDTH;
	height = HEIGHT;

	// Open files...

	f_pic_vq2_indices=fopen(image1,"rb");
	fread((void*)CodevectorIndices,sizeof(short int),(size_t)pels,f_pic_vq2_indices);
	
	f_pic_ex_wmark=fopen(image2,"wb");
		
	for(row=0;row<height;row++){											
		for(col=0;col<width;col++){
			check = CodevectorIndices[row][col];
			check = check%2;
			if (check==1)						// Odd/Even Check.
				ExtractedWatermark[row][col] = 1;
			else if (check==0)
				ExtractedWatermark[row][col] = 0;
			}
	}
	
	fwrite((void*)ExtractedWatermark,sizeof(short int),(size_t)pels,f_pic_ex_wmark);		// Writing pixels to file
	fclose(f_pic_vq2_indices);
	fclose(f_pic_ex_wmark);
	
	printf("\nExtracted SF Watermark from Indices\n");
	//getchar();
}
