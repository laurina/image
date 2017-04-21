/*************************************************************************
Sample C Programs for Computer Project of Digital Image Processing.
These programs provide image reading and writing functions.
Students should modify and extend these programs for the project.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

 char fin_name[] = {"picture_1_LPF_3.bmp"};
 char fout_name[] = {"picture_1_LPF_3_Laplacian_WithNo2.bmp"};
 unsigned char  *fin;

//unsigned char GetMedianNum(unsigned char * bArray, int iFilterLen); 
unsigned char *bmpread(char *imname, int *n_row, int *n_col);
void bmpsave(char *imname, unsigned char *ima, int n_row, int n_col);
void MiddleFilterProcessing( unsigned char *f,int row,int col,int len);
void ImageMaskProcessing(unsigned char *f, int row, int col,int *Mask, int MaskWH, int MaskCoff);
void GuassLowPassFilterProcessing(unsigned char *f, int row,int col);

int main(int argc, char *argv[])
{
  int   i, j, n_row, n_col,filerlen;
  int  n,m,w,b;
  
  unsigned char  *ima;

  /* Read image */

  ima = bmpread(fin_name, &n_row, &n_col);
 
  fin =(unsigned char *)calloc(n_row * n_col,sizeof(unsigned char));
  /* Show the image size */

  printf("Number of rows = %d, Number of columns = %d\n", n_row, n_col);

  /* Reduce pixel intensity by a factor of 2. This is just a simple example
     and you you need to do things much more complicated than this. */

  /********** Salt and Pepper **********
  srand((int)time(0));
  
     // ima[i*n_col+j] /= 2;}}

 for( w = 0;w<3000;w++){
	  m=rand()%n_row;
	  n=rand()%n_col;
	  ima[m*n_col+n]=255;
  }

  for(b = 0; b < 3000; b++){
	  m=rand()%n_row;
	  n=rand()%n_col;
	  ima[m*n_col+n]=0;
  }
	    
********************************************/
  /* Save output */
  filerlen = 3;
 // MiddleFilterProcessing(ima,n_row,n_col,filerlen);
  GuassLowPassFilterProcessing(ima,n_row,n_col);

  bmpsave(fout_name, fin, n_row, n_col);
  
  return 0;
}

void ImageMaskProcessing(unsigned char *f, int row, int col,int *Mask, int MaskWH, int MaskCoff)
{
	int Coff;	
	int i, j, m, n;	
	int k;
	k = (MaskWH-1)/2;
	for (i = k; i < row-k; i++){
		for (j = k; j < col-k; j++) {
		    Coff = 0; 
		    for (m=-k; m<=k; m++) {
				for (n=-k; n<=k; n++) {
			    Coff += f[(i+m) * col+(j+n)] * Mask[(m+k) * MaskWH +(n+k)];
				}
			}

			fin[i * col + j] = (unsigned char)Coff;
			if (Coff < 0) 
				fin[i*col + j] = 0;
			if(Coff >255)
				fin[i*col + j] = 255;
			/****    For LowPassFilter
			fin[i * col + j] = (unsigned char) (Coff / MaskCoff);
			if (Coff < 0) 
				fin[i*col + j] = 0;
			*/
		}
	}
}

void GuassLowPassFilterProcessing(unsigned char *f, int row,int col)
{
	//int Mask[9];
	//int i ;
	
	int MaskGLP[9] = {1, 2, 1, 
					  2, 4, 2, 
			          1, 2, 1};
	int MaskLWD1[9] = {1, 1, 1, 
					   1,-8, 1, 
			           1, 1, 1};
	int MaskLWN1[9] = {0, 1, 0, 
					   1,-4, 1, 
			           0, 1, 0};
	int MaskLWD2[9] = {-1,-1,-1, 
					   -1, 8,-1, 
			           -1, 1,-1};
	int MaskLWN2[9] = {0,-1, 0, 
					  -1, 4,-1, 
			           0,-1, 0};

/*
	for(i = 0;i < 9; i++){
		Mask[i] = 1;
	}*/
	
	ImageMaskProcessing(f,row,col, MaskLWN2, 3, 0);
}

void MiddleFilterProcessing(unsigned char *f,int row,int col,int len)
{	
	int i, j, m, n, k, l,halflenth,mlength;
	int  Middle, MiddlePos; 
	unsigned char *vSort;
	vSort =(unsigned char *)calloc(len * len,sizeof(unsigned char));
	halflenth = (int)(len/2);
	mlength = (int)(len*len/2)+1;

	for (i=halflenth; i<row-halflenth; i++) {
		for (j=halflenth ; j<col-halflenth; j++) {l = 0;
		     for (m = -halflenth; m <= halflenth; m++) {				
		          for (n = -halflenth; n <= halflenth; n++) {
					  vSort[l++] = f[(i + m)*col+(j + n)];
				  }
			 }
			 
			 		 
			 for (k=0; k < mlength; k++) {			
				 Middle = vSort[k]; 
				 MiddlePos = k;
				 for (l=k; l<len*len; l++) {
					 if (Middle > vSort[l]){
						 Middle = vSort[l];
						 MiddlePos = l;
					 }
				 }
				 vSort[MiddlePos] = vSort[k];
				 vSort[k] = Middle;
			 }
			 fin[i*col+j] = (unsigned char) Middle;
		}
	}
}



unsigned char *bmpread(char *imname, int *n_row, int *n_col)
{
  int            i, ncols, nrows, n_bit, n_tab, inv, mskip;
  short          word1;
  unsigned int   uint1;
  unsigned char  byte1, byte2, *ima;
  FILE           *imdata;

  imdata = fopen(imname, "rb");
  if (imdata == NULL) {
    printf("%s open error.\n", imname);  exit(0);
  }

  fread(&byte1, 1, 1, imdata);  fread(&byte2, 1, 1, imdata);
  if (byte1 != 'B' || byte2 != 'M') {
    printf("Not a BMP file.\n");  exit(0);
  }

  fseek(imdata, 18, 0);

  fread(&ncols, 4, 1, imdata);  fread(&nrows, 4, 1, imdata);
  if (nrows < 0) {nrows = -nrows;  inv = 1;}  else inv = 0;

  fseek(imdata, 2, 1);
  fread(&word1, 2, 1, imdata);   n_bit = word1;
  if (n_bit != 8) {
    printf("n_bit = %d not supported.\n", n_bit);  
	exit(0);
  }

  fread(&uint1, 4, 1, imdata);
  if (uint1 != 0) {
    printf("Compression not supported.\n");  exit(0);
  }

  fseek(imdata, 12, 1);
  fread(&uint1, 4, 1, imdata);  n_tab = uint1;
  fread(&uint1, 4, 1, imdata);

  //Although in the bmp file, n_tab (no. of colours in the colour palette) is 0, Windows BMP file format still fills 
  //256 entries colour table with 4byte per entry in the file, so we have to skip this 4*256 bytes colour table.
  //fseek(imdata, 4*n_tab, 1);
  fseek(imdata, 4*256, 1);

  ima = (unsigned char *)malloc(nrows*ncols*sizeof(unsigned char));
  if (ima == NULL) {
    printf("Not enough memory\n");  exit(1);
  }

  mskip = ncols/4;  if (4*mskip < ncols) mskip++;  mskip = 4*mskip - ncols;

  if (inv == 1) {
    for (i=0; i<nrows; i++) {
      fread(&(ima[i*ncols]), 1, ncols, imdata);
      fseek(imdata, mskip, SEEK_CUR);
    }
  }
  else {
    for (i=nrows-1; i>=0; i--) {
      fread(&(ima[i*ncols]), 1, ncols, imdata);
      fseek(imdata, mskip, SEEK_CUR);
    }
  }

  fclose(imdata);

  *n_col = ncols;  *n_row = nrows;

  return ima;
}


void bmpsave(char *imname, unsigned char *ima, int n_row, int n_col)
{
  int            i, j, mskip;
  unsigned int   uint1;
  short          word1;
  unsigned char  byte1, byte2;
  FILE           *imdata;

  imdata = fopen(imname, "wb");
  if (imdata == NULL) {
    printf("%s open error.\n", imname);  exit(0);
  }

  byte1 = 'B';  byte2 = 'M';
  fwrite(&byte1, 1, 1, imdata);  fwrite(&byte2, 1, 1, imdata);
  uint1 = 54 + 4*256 + n_col*n_row;  fwrite(&uint1, 4, 1, imdata);
  word1 = 0;   fwrite(&word1, 2, 1, imdata);  fwrite(&word1, 2, 1, imdata);
  uint1 = 54 + 4*256;  fwrite(&uint1, 4, 1, imdata);  
  uint1 = 40;  fwrite(&uint1, 4, 1, imdata);
  fwrite(&n_col, 4, 1, imdata);  fwrite(&n_row, 4, 1, imdata);
  word1 = 1;  fwrite(&word1, 2, 1, imdata);
  word1 = 8;  fwrite(&word1, 2, 1, imdata);
  uint1 = 0;  fwrite(&uint1, 4, 1, imdata);

  mskip = n_col/4;  if (4*mskip < n_col) mskip++;  mskip = 4*mskip - n_col;
  uint1 = (n_col+mskip)*n_row;  fwrite(&uint1, 4, 1, imdata);
  j = 11811;  fwrite(&j, 4, 1, imdata);  fwrite(&j, 4, 1, imdata);
  uint1 = 256;   fwrite(&uint1, 4, 1, imdata);
  uint1 = 0;     fwrite(&uint1, 4, 1, imdata);

  for (i=0; i<256; i++) {
    byte1 = i;
    fwrite(&byte1, 1, 1, imdata);  fwrite(&byte1, 1, 1, imdata);
    fwrite(&byte1, 1, 1, imdata);  fwrite(&byte1, 1, 1, imdata);
  }

  byte1 = 0;
  for (i=n_row-1; i>=0; i--) {
    fwrite(&(ima[i*n_col]), 1, n_col, imdata);
    if (mskip != 0) {
      for (j=0; j<mskip; j++) fwrite(&byte1, 1, 1, imdata);
    }
  }

  fclose(imdata);
}




