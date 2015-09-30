// Checks if the sequences of a given source are easily compressible, and the maximum entropy that could have a source with the same number of symbols as the given.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define IZQ 3
#define DER 2
#define EXIST 1


struct Table//matriz con el simbolo y el número de veces que se repite ese simbolo en el archivo
{
	int nvecs;
	unsigned char *simbl;
	float prob;
	struct Table *izq;
	struct Table *der;
};
typedef struct Table Table;

void PreOrden(Table *,int ,double &);

void Talar(Table *);
int existe(int , Table **, char *);
int comprueba(unsigned char *, Table *,int ,int );

int main (int argc, char * const argv[])
{
    Table tabla,*lista;
	Table *pre;
	FILE *font;//archivo fuente
	long comienzo,final,j;
	int nbloc,N;
	unsigned char *cmp;
	int nsimb,r;
	char *buff;
	double ht;

	  if ((font= fopen(argv[1],"rt") )== NULL)
	  {
		printf("Error al abrir el archivo, compruebe la sintaxis del programa y que el fichero existe\n");
		printf("\t\t entropia <nombre del archivo> <número de bytes>\n\n");
		exit(0);
	  }
	  else 
	  {
		comienzo=ftell(font);
		fseek(font,0,SEEK_END);
		final=ftell(font);
	  }

	N=atoi(argv[2]); //secuencias de N bytes
			if(N<=0)
		{
			system("/usr/bin/clear");
			printf("\tIntroduzca valores de N enteros y mayores que 0\n");
			exit(0);
		}
	
	nbloc=(final-comienzo)/N;	//número de bloques de 1byte	
	fseek(font,0,SEEK_SET);
	buff=(char *)malloc(sizeof(char)*(final-comienzo));
		fread(buff,sizeof(char),final-comienzo,font);
	fclose(font);
	tabla.simbl=(unsigned char *)malloc(sizeof(unsigned char)*N);
	cmp=(unsigned char *)malloc(sizeof(unsigned char)*N);
	lista=NULL;
	memmove(tabla.simbl,&buff[0],N);
	tabla.der=NULL;
	tabla.izq=NULL;
	lista=&tabla;
	nsimb=1;
	pre=lista;

	for(j=0;j<=((nbloc-1));j++)
	{

		memmove(cmp,&buff[(j*N)],N);
		r=memcmp(tabla.simbl,cmp,sizeof(unsigned char)*N);
		if(r==0) tabla.nvecs++;
		else
			{
			if(r=comprueba(cmp,lista,nsimb,N)!=EXIST) nsimb++;
			}
	}
	ht=0;

	PreOrden(lista,nbloc,(ht));
	Talar(lista);
	free(buff);
	free(cmp);
	free(tabla.simbl);
	
	printf("%1.5f",ht);
	printf("\t %1.5f\n",log2(nsimb));
	
}
int comprueba(unsigned char *cmpc,Table *lista,int nsimbc,int N)
{
	Table *letra,*anterior,*aux;
	int r;
	int exist=0;
	int e;

	letra=lista;
	(*letra).nvecs=1;
	anterior=letra;
	while(letra!=NULL)
	{
		anterior=letra;
			
			if(r=memcmp((*letra).simbl,cmpc,sizeof(char)*N)==0)
				{
				(*letra).nvecs++;
				exist=EXIST;
				break;
				}
			else
			{
				if(r==memcmp((*letra).simbl,cmpc,sizeof(char)*N)>0) {
					letra=(*letra).der;
					e=DER;
				}
				else{
					letra=(*letra).izq;
					e=IZQ;
				}
			}
	}
	
				if(exist!=EXIST)
				{
					aux=(Table *)malloc(sizeof(Table));
					(*aux).simbl=(unsigned char *)malloc(sizeof(char)*N);
					memmove((*aux).simbl,cmpc,N);
					(*aux).nvecs=1;
					(*aux).der=NULL;
					(*aux).izq=NULL;
				if(e==DER) (*anterior).der=aux;
				if(e==IZQ) (*anterior).izq=aux;
				}

return(exist);
}
void PreOrden(Table *aux, int nbloc,double &ht)
{
	double h;
	
	(*aux).prob=(float)((*aux).nvecs)/(float)(nbloc);
	h=((*aux).prob)*(log2(1/(*aux).prob));
	(ht)=(ht)+h;

	if(aux->der!=NULL) PreOrden(aux->der,nbloc,ht);
	if(aux->izq!=NULL) PreOrden(aux->izq,nbloc,ht);
}
void Talar(Table *aux)
{
   if((aux)!=NULL)
  { // Algoritmo recursivo, recorrido en postorden
		Talar((aux)->izq);
		Talar((aux)->der);
	// Eliminar nodo
      free((*aux).der);
	  free((*aux).izq);
	  aux = NULL;
	}
}
