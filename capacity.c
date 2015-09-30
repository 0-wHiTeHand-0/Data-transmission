// BSC (binary symmetric channel) simulation that calculates the capacity of such channel in a practical way.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

struct nodo{
  char *simbolo;
  struct nodo *siguiente;
};

int main(int argc,char *argv[]){
  float error,perror,aleat, v3=0;
  int k,k2,n,i,rep,nsims, valor,var,numb,valor_ant=0;
  struct nodo *a,*b,*c,*d;
  char *escogido,*crea_nodo;
 
  error=atof(argv[1]);
  k=atoi(argv[2]);
  n=atoi(argv[3]);
  nsims=atoi(argv[4]);
  k2=pow(2,k);
  a=b=c=NULL;
  escogido=(char*)malloc(sizeof(char)*(n+1));
  crea_nodo=(char*)malloc(sizeof(char)*(n+1));
  a=(struct nodo*)malloc(sizeof(struct nodo));
  a->simbolo=(char*)malloc(sizeof(char)*(n+1));
  srand(time(NULL));
  for (i=0; i<n; i++){ 
	a->simbolo[i]=crea_nodo[i];
    crea_nodo[i]=random()%2;
  }
  a->siguiente=NULL;
  b=a;
  for (rep=1; rep<k2; rep++){
	b->siguiente=(struct nodo*)malloc(sizeof(struct nodo));
    b=b->siguiente;
    b->simbolo=(char*)malloc(sizeof(char)*(n+1));
    for (i=0; i<n; i++){
	  b->simbolo[i]=crea_nodo[i];
      crea_nodo[i]=random()%2;
    }
    b->siguiente=NULL;
  }
  b=a;
    //hasta aqui se crea el codigo de manera aleatoria
  for(rep=0;rep<nsims;rep++){
    b=a;
    i=random()%(k2+1);
    numb=1;
    if(i!=0){
      while (numb!=i){
	b=b->siguiente;
	numb++;
      }
    }
    var=0;
    c=b;
    while(var!=n){    //escojo los simbolos aleatoriamente
      escogido[var]=b->simbolo[var];
      var++;
    }
    i=0;
    while(i!=n) {
      aleat=rand()%100;
      perror=error*100;
      if ((escogido[i]==0) && (aleat<perror)) escogido[i]=1;
      else if (aleat<perror) escogido[i]=0; //cambio los bits de la palabra aleatoriamente
      i++;
    }
    b=a;
    valor_ant=0;
    while(b->siguiente!=NULL){
      valor=var=0;
      while(var!=n){//por cada bit que coincida de un simbolo con el cambiado se incrementa valor
		  if(escogido[var]==b->simbolo[var]) valor++; 
		  var++;
      }
      if(valor>valor_ant){
	d=b;
	valor_ant=valor;
      }
      if (valor==n) break;    
      b=b->siguiente;
    }
    var=valor=0;
    while(var!=n){
      if(c->simbolo[var]==d->simbolo[var]) valor++; 
      var++;
    }
    if(valor==n) v3++;
  }
  printf("%f ",((float)n/(float)k)); //se muestran los resultados y sale.
  printf("%f\n",(1-(v3/(float)nsims)));
  exit(0);
}
