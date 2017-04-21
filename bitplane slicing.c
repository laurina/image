/*************************************************************************
Sample C Programs for Computer Project of Digital Image Processing.
These programs provide image reading and writing functions.
Students should modify and extend these programs for the project.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>


unsigned char fin_name[] = {"picture_1.bmp"};
unsigned char fout_name1[] = {"picture_1_reconstruct_Bit67.bmp"};
unsigned char fout_name2[] = {"picture_1_reconstruct_MSB.bmp"};
unsigned char fout_name3[] = {"picture_1_reconstruct_LSB.bmp"};
unsigned char fout_name4[] = {"picture_1_reconstruct_MSB=0.bmp"};
unsigned char fout_name5[] = {"picture_1_reconstruct_LSB=0.bmp"};


unsigned char *bmpread(char *imname, int *n_row, int *n_col);
void bmpsave(char *imname, unsigned char *ima, int n_row, int n_col);

int main(int argc, char *argv[])
{
  int   i, j, n_row, n_col,n,rang_min,rang_max,m,rrang_max,rrang_min,k;
  unsigned char  *ima,*temp,*temp2,*f,*combinedData,*LSB_0_Data,*MSB_0_Data,*data;


  

  /* Read image */

  ima = bmpread(fin_name, &n_row, &n_col);
 
  temp =(unsigned char *)calloc(n_row*n_col,sizeof(unsigned char));
  temp2 =(unsigned char *)calloc(n_row*n_col,sizeof(unsigned char));
  f =(unsigned char *)calloc(n_row*n_col,sizeof(unsigned char));

   data = (unsigned char *)malloc(n_row*n_col*sizeof(unsigned char));
    combinedData=(unsigned char*)malloc(n_row*n_col*sizeof(unsigned char));
    LSB_0_Data=(unsigned char *)malloc(n_row*n_col*sizeof(unsigned char));
    MSB_0_Data=(unsigned char *)malloc(n_row*n_col*sizeof(unsigned char));
  /* Show the image size */

  printf("Number of rows = %d, Number of columns = %d\n", n_row, n_col);


  /* Reduce pixel intensity by a factor of 2. This is just a simple example
     and you you need to do things much more complicated than this. */

  /**************   Bit Plane Slicing    **************/

   
    for( k=0;k<8;k++){
        for( i=0;i<n_row;i++){
            for( j=0;j<n_col;j++){
                data[i*n_col+j]=getBitGrayLevel((ima[i*n_col+j]), k);
                if(k==7) {
                    combinedData[i*n_col+j]=combine(ima[i*n_col+j]);
                    LSB_0_Data[i*n_col+j]=LSB_0(ima[i*n_col+j]);
                    MSB_0_Data[i*n_col+j]=MSB_0(ima[i*n_col+j]);
                }
            }
        }
		if(k==0){
			bmpsave(fout_name3, data, n_row, n_col);
		}
		if(k==7){
			bmpsave(fout_name2, data, n_row, n_col);
		}
    }
    bmpsave(fout_name1, combinedData, n_row, n_col);
    bmpsave(fout_name5, LSB_0_Data, n_row, n_col);
    bmpsave(fout_name4, MSB_0_Data, n_row, n_col);



  /* Save output */
  
 // bmpsave(fout_name, temp , n_row, n_col);

  return 0;
}



int getBitGrayLevel(int grayLevel,int level){
    int color=0;
    switch(level){
        case 0:
            color=grayLevel&1;  // 0000 0001
            if(color==1){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            break;
        case 1:
            color=grayLevel&0x2;  // 0000 0010
            if(color==0x2){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            break;
        case 2:
            color=grayLevel&0x4; // 0000 0100
            if(color==0x4){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            break;
        case 3:
            color=grayLevel&0x8; // 0000 1000
            if(color==0x8){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            
            break;
        case 4:
            color=grayLevel&0x10; // 0001 0000
            if(color==0x10){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            
            break;
        case 5:
            color=grayLevel&0x20; // 0010 0000
            if(color==0x20){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            
            break;
        case 6:
            color=grayLevel&0x40; // 0100 0000
            if(color==0x40){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            
            break;
        case 7:
            color=grayLevel&0x80; // 1000 0000
            if(color==0x80){
                grayLevel=255;
            }else{
                grayLevel=0;
            }
            
            break;
    }
    return grayLevel;
}




int combine(int grayLevel){  
    int colors;
    
    colors=grayLevel & 0xC0;  // 1100 0000
    return  colors;
}


int LSB_0(int grayLevel){     
    int colors;
    
    colors=grayLevel & 0xFE; // 1111 1110  
    
    return colors;
}



int MSB_0(int grayLevel){                 
    int colors;
    
    colors=grayLevel & 0x7F; // 0111 1111   
    
    return colors;
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


