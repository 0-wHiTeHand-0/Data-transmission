// CRC-32 implementation using the polynomial reverse representation. Used in Ethernet, PNG, V.42, MPEG2, gzip and many other technologies.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	FILE *f;
	unsigned char trama[10000], biest;
	unsigned int lee=10000, x, y, byte=0, resto;
	
	strcpy(trama, argv[1]);
	
	if ((f=fopen(trama, "rb")) == NULL) {
		printf("El fichero no existe\n");
		exit(1);
	}

	while((lee==10000) && (!feof(f))){
		lee=fread(trama,1,10000,f);/*Lee de 10 kb en 10 kb para aumentar la rapidez*/
		for (x=0; x<lee; x++) {
			biest=trama[x];
			for (y=0; y<8; y++) {
				resto=byte&(1);/*es el resto para sumarlo al bit que entra*/
				byte=byte>>1;
				if((((biest&(128))!=0) && (resto==0)) || (((biest&(128))==0) && (resto!=0))) byte=byte^0xEDB88320;
				biest=biest<<1;
			}
		}
	}
	printf("%08X\n", byte);
	fclose(f);
	exit(0);
}
