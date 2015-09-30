// CRC-16-CCITT calculation using the polynomial reverse representation. Used in Bluetooth, SD, X.25 and many other technologies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
  int x;
  unsigned short int biest=0, r=0;
  long s,num=10000;
  unsigned char byte_2='0';             
  char nombre_fich[20];
  FILE *f;
  unsigned char trama[10000];
  long int siguiente,total=0;
  
  strcpy(nombre_fich,argv[1]);
  
  if ((f=fopen(nombre_fich,"rb"))==NULL){
	 printf("El fichero de entrada no existe\n");
	 exit(1);
  }
  while (num==10000){
    num=fread(trama,sizeof(char),10000,f);
    for(s=0;s<num;s++){
      byte_2=trama[s];
      for(x=0;x<8;x++){
       	r=biest&(1);
		biest=biest>>1;
    	if (((byte_2 >127)&&(r==0)) || ((byte_2 <=127)&&(r!=0))) biest=biest^0x8408;
	    byte_2=byte_2<<1;
	  }
    }
  }
printf("%04X\n",biest);
  fclose(f);
  exit(0);
}  


