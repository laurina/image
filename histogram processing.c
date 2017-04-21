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



unsigned char fin_name[] = {"picture_0_Gamma_2.bmp"};
unsigned char fout_name[] = {"picture_0_Gamma_2_equalization.bmp"};
 

unsigned char *bmpread(char *imname, int *n_row, int *n_col);
void bmpsave(char *imname, unsigned char *ima, int n_row, int n_col);

int main(int argc, char *argv[])
{
  int   i, j, n_row, n_col,max,min;
  double temp;
  unsigned char  *ima,*s,*t,*k;
  

  int n[256]={0};
  int m[256]={0};
  double p[256]={0};
  double c[256]={0};

  /* Read image */

  ima = bmpread(fin_name, &n_row, &n_col);
  t=(unsigned char *)calloc(n_row*n_col,sizeof(unsigned char));//分配一个长度width高度为height内存空间给指向指针
  k=(unsigned char *)calloc(n_row*n_col,sizeof(unsigned char));

 
  /* Show the image size */

  printf("Number of rows = %d, Number of columns = %d\n", n_row, n_col);

  /* Reduce pixel intensity by a factor of 2. This is just a simple example
     and you you need to do things much more complicated than this. */

  for (i=0; i<n_row; i++) {
    for (j=0; j<n_col; j++) {

	  n[ima[i*n_col+j]]++; 
	  //将灰度值相同的进行统计，并从小到大存在数组n[256]中

		/* *********  Gamma  ***************

		s = pow(ima[i*n_col+j]/255.0,2)*255;
		 // 如果超界则修改其值 
        if (s < 0) 
            s = 0.0; 
        else if (s > 255) 
            s = 255;   

		ima[i*n_col+j] = s;    
		*/
   } 	  
  }

  for(i=0;i<256;i++){
	  p[i] = (double)n[i]/(n_row * n_col);//将灰度值为i的个数除以图像的总像素点，也就是灰度值为i的概率密度p(i) 
	}


  for(i=0;i<256;i++){
	  for(j=0;j<=i;j++){
		  c[i] += p[j]; //生成灰度值为i的概率分布c(i)
	  }
  }

  max = min = ima[0];

  for(i=0;i<n_row;i++){
	  for(j=0;j<n_col;j++){
		  if(max<ima[i*n_col+j]){
			  max=ima[i*n_col+j];
		  }
		  else if(min > ima[i*n_col+j]){
			  min = ima[i*n_col+j];
		  }
	  }
  }//算出灰度值范围，最大值max和最小值min
  
  for(i = 0;i < n_row;i++){
	  for(j = 0;j < n_col;j++){
		  t[i*n_col+j]=(int)(c[ima[i*n_col+j]]*max+0.5);
		  //将图像中像素点的灰度值对应的概率分布函数乘以灰度值范围再四舍五入，把算出来的新的灰度值赋给t 
	  }
  }

  /*
  for(i = 0;i < n_row;i++){
	  for(j = 0;j < n_col;j++){
		  m[t[i*n_col+j]]++;
	  }
  }
 
  for(i = 0;i < n_row;i++){
	  for(j = 0;j < n_col;j++){
  //for(i=0;i<256;i++){
	  printf("%hhu\n",t[i*n_col+j]);
  }
  }
  */
  /* Save output */

  bmpsave(fout_name, t, n_row, n_col);

  system("pause");
 
  return 0;
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


