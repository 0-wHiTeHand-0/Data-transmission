// Calculates how much can a file be compressed using the Huffman code. Implementation made with binary trees.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Hoja{
	unsigned char *p_lec;
	double contador;
	double probabilidad;
	int caracter;
	int s;
	struct Hoja *continua;
	struct Hoja *drch;
	struct Hoja *izqd;
};

//Crea una hoja nueva del arbol y devuelve el puntero a la misma.
struct Hoja* crear_hoja ( unsigned char* p_mem, int n){

	struct Hoja* hoja = (struct Hoja*)malloc(1*sizeof(struct Hoja));
	hoja->p_lec = (unsigned char *) malloc(n*sizeof(unsigned char));
	memcpy(hoja->p_lec,p_mem,n);
	hoja -> drch = NULL;
	hoja -> izqd = NULL;
	hoja -> contador = 1;
	hoja -> probabilidad = 0;

	return (hoja);
}

//Crea una hoja nueva del arbol de Prob y devuelve el puntero a la misma.
struct Hoja* crear_hojaProb ( struct Hoja* h, int n){

	struct Hoja* hoja = (struct Hoja*)malloc(1*sizeof(struct Hoja));
	hoja->p_lec = (unsigned char *) malloc(n*sizeof(unsigned char));
	memcpy(hoja->p_lec,h->p_lec,n);
	hoja -> drch = NULL;
	hoja -> izqd = NULL;
	hoja -> probabilidad = h->probabilidad;
	hoja -> contador = h->contador;
	return (hoja);
}

// Comparacion de la lectura con las hojas del arbol
// Incremento del contador de la hoja en caso de igualdad
// Creacion nueva hoja en caso necesario.
void comparacion ( unsigned char *p_mem, struct Hoja* hoja, int n){

	int comp = memcmp (hoja->p_lec,p_mem,n);
	int sigue = 0;
	//printf("Comparacion: %i\n",comp);
	if(comp==0){
		//printf("= 0\n");
		hoja -> contador ++; //Incremento contador
	}else if( comp > 0 ){ // si es mayor
		if( hoja -> drch != NULL){
			//printf("hay hoja drch\n");
			hoja = hoja -> drch;
			sigue = 1;
		}else{
			//printf("No hay hoja drch\n");
			//Crea una nueva hoja y la enlaza con la actual.
			hoja -> drch = crear_hoja( p_mem, n);
		}
	}else{
		if( hoja -> izqd != NULL){
			//printf("Hay hoja izq\n");
			hoja = hoja -> izqd;
			sigue = 1;
		}else{
			//printf("No hay hoja izq\n");
			//Crea una nueva hoja y la enlaza con la actual.
			hoja -> izqd = crear_hoja( p_mem, n);
		}
	}

	if(sigue){
		//LLamada a comparacion
		comparacion(p_mem, hoja, n);
	}
	//printf("Fin comparacion\n");
}

// Crea el arbol ordenado por Prob
void crearArbolProb(struct Hoja* h, struct Hoja** hProb, struct Hoja** RaizProb, int n){
	int sigue = 1;
	if(*hProb == NULL){
		//printf("No hay hoja cabecera\n");
		*hProb = crear_hojaProb(h,n);
		//printf("Crea hoja raiz Prob\n");
		sigue = 0;

		//Copio la cabecera en raiz
		*RaizProb = *hProb;
	}

	if(sigue){
		if( h->probabilidad > (*hProb)->probabilidad ){ // Si es mayor
			//printf("Es mayor\n");
			if((*hProb)-> drch != NULL){
				//printf("hay hoja drch\n");
				*hProb = (*hProb)-> drch;
			}else{
				//printf("No hay hoja drch\n");
				//Crea una nueva hoja y la enlaza con la actual.
				(*hProb) -> drch = crear_hojaProb(h,n);
				//printf("Crea hoja arbol Prob\n");
				sigue = 0;
			}
		}else{										// Si es menor o =
			//printf("NO es mayor\n");
			if( (*hProb) -> izqd != NULL){
				//printf("Hay hoja izq\n");
				*hProb = (*hProb) -> izqd;
			}else{
				//printf("No hay hoja izq\n");
				//Crea una nueva hoja y la enlaza con la actual.
				(*hProb) -> izqd = crear_hojaProb(h,n);
				//printf("Crea hoja arbol Prob\n");
				sigue = 0;
			}
		}
	}
	if(sigue){
		crearArbolProb(h, hProb,RaizProb, n);
	}
}

//Recorre el Arbol y crea el nuevo arbol por probabilidad
void recorrerArbol(struct Hoja* h, int* contador_hojas, int num_lec,struct Hoja** hProb,struct Hoja** RaizProb, int n){
	float prob_i;//Variable para la probabilidad de cada elemento individual.
	//printf("Nueva hoja arbol orig\n");
	if (h != NULL) {
		*contador_hojas = *contador_hojas +1;     //Cuento el numero de hojas

		prob_i = (h->contador/num_lec); //Calculamos la probabilidad individual de cada simbolo independiente
		h->probabilidad = prob_i;

		//Inserto el nodo en el nuevo arbol prob.
		//Pasar cabecera en hProb.

		if (*hProb!=NULL){
			*hProb = *RaizProb;
		}
		crearArbolProb(h,hProb, RaizProb, n);

		recorrerArbol(h->izqd,contador_hojas, num_lec,hProb,RaizProb,n);

		recorrerArbol(h->drch,contador_hojas, num_lec,hProb,RaizProb,n);
	}
}

void crearLista (struct Hoja* hoja, struct Hoja** inicioLista, struct Hoja** finalLista, struct Hoja** recorreLista,int n){

	if(hoja == NULL){
		printf("Error: Arbol vacio\n");
		exit(0);
	}
	//recorremos hasta izquierda
	if (hoja->izqd != NULL){
		//printf("HOLAAA 0\n");
		crearLista(hoja->izqd, inicioLista, finalLista, recorreLista, n);
	}
	//introducimos en lista el menor y sucesivos menores hasta ver derecha con algo
	if (*inicioLista != NULL){

		(*recorreLista)->continua = (struct Hoja*) malloc(1*sizeof(struct Hoja));
		(*recorreLista) = (*recorreLista)->continua;
		(*recorreLista)->p_lec = (unsigned char*) malloc(n*sizeof(unsigned char));
		memcpy((*recorreLista)->p_lec,hoja->p_lec,n);
		(*recorreLista)->probabilidad = hoja->probabilidad;
		(*recorreLista)->contador = hoja->contador;
		(*recorreLista)->drch = NULL;
		(*recorreLista)->izqd = NULL;
		(*recorreLista)->continua = NULL;
		(*recorreLista)->s=1;
		*finalLista = *recorreLista;
		//printf("HOLAAAAAA 1\n");
	}

	if (*inicioLista == NULL){
		*inicioLista = (struct Hoja*) malloc(1*sizeof(struct Hoja));
		(*inicioLista)->p_lec = (unsigned char*) malloc(n*sizeof(unsigned char));
		memcpy((*inicioLista)->p_lec, hoja->p_lec,n);
		(*inicioLista)->probabilidad = hoja->probabilidad;
		(*inicioLista)->contador = hoja->contador;
		(*inicioLista)->drch = NULL;
		(*inicioLista)->izqd = NULL;
		(*inicioLista)->continua = NULL;
		(*inicioLista)->s=1;
		*recorreLista = *inicioLista;
		*finalLista = *inicioLista;
		//printf("HOLAAAAAA 2\n");
	}
	//reiteramos en la rama de la derecha el proceso previo mediante recursividad.
	if (hoja->drch != NULL){
		//printf("HOLAAAAA 3\n");
		crearLista(hoja->drch, inicioLista, finalLista, recorreLista, n);
		//printf("HOLAAAAAAAAA 4\n");
	}
	//printf("salgo\n");
}


void crearListaVector (float vector[], struct Hoja** inicioLista, struct Hoja** finalLista, struct Hoja** recorreLista, int num_lecturas, int n){
	//printf("Entro en lista_vect\n");

	int i;
	double prob_i;
	struct Hoja *actual;

	for( i=0; i<256; i++ ){ //Recorremos el Array.

		if( vector[i] != 0 ){ //Calculamos solo para los casos que sean != 0

			prob_i = (vector[i]/num_lecturas); //Calculamos la probabilidad individual de cada simbolo independiente
			if (*inicioLista == NULL){
				*inicioLista = (struct Hoja*) malloc(1*sizeof(struct Hoja));
				(*inicioLista)->p_lec = NULL;
				(*inicioLista)->caracter = i;
				(*inicioLista)->contador = vector[i];
				(*inicioLista)->probabilidad = prob_i;
				(*inicioLista)->continua = NULL;
				(*inicioLista)->s=1;
				*recorreLista = *inicioLista;
				*finalLista = *inicioLista;
				//printf("No hay iniciolista \n");
			}else{
				if((*inicioLista)->probabilidad>prob_i){
					*recorreLista = (struct Hoja*) malloc(1*sizeof(struct Hoja));
					(*recorreLista)->continua = (*inicioLista);
					(*recorreLista)->p_lec = NULL;
					(*recorreLista)->caracter = i;
					(*recorreLista)->contador = vector[i];
					(*recorreLista)->probabilidad = prob_i;
					(*recorreLista)->drch = NULL;
					(*recorreLista)->izqd = NULL;
					(*recorreLista)->s=1;
					*inicioLista = *recorreLista;
					//printf("nuevo nodo al inicio \n");
				}else if((*finalLista)->probabilidad<prob_i){
					*recorreLista = *finalLista;
					(*recorreLista)->continua = (struct Hoja*) malloc(1*sizeof(struct Hoja));
					(*recorreLista) = (*recorreLista)->continua;
					(*recorreLista)->p_lec = NULL;
					(*recorreLista)->caracter = i;
					(*recorreLista)->contador = vector[i];
					(*recorreLista)->probabilidad = prob_i;
					(*recorreLista)->drch = NULL;
					(*recorreLista)->izqd = NULL;
					(*recorreLista)->s=1;
					*finalLista = *recorreLista;
					//printf("nuevo nodo al final \n");
				}else{
					*recorreLista = *inicioLista;
					while((*recorreLista)->continua->probabilidad<prob_i){
						(*recorreLista) = (*recorreLista)->continua;
					}
					actual = (struct Hoja*) malloc(1*sizeof(struct Hoja));
					actual->continua = (*recorreLista)->continua;
					(*recorreLista)->continua = actual;
					(*recorreLista) = (*recorreLista)->continua;
					(*recorreLista)->p_lec = NULL;
					(*recorreLista)->caracter = i;
					(*recorreLista)->contador = vector[i];
					(*recorreLista)->probabilidad = prob_i;
					(*recorreLista)->drch = NULL;
					(*recorreLista)->izqd = NULL;
					(*recorreLista)->s=1;
					//printf("nuevo nodo intermedio \n");
				}
			}
		}
	}
	//printf("Salgo de lista_vect\n");
}

void crearArbolHuffman(struct Hoja** raizHuffman, struct Hoja* inicioLista, struct Hoja* finalLista, struct Hoja* recorreLista){

	//Comprobamos si existe la lista
	if(inicioLista == NULL){
		printf("Error: Lista no existente al crear el arbol de huffman\n");
		exit(1);
	}
	//comprobamos si final de lista

	if(inicioLista->continua == NULL){
		//printf("Estoy en en el caso inicioLista->continua == NULL\n");
		struct Hoja* sumaDos = (struct Hoja*)malloc(1*sizeof(struct Hoja));
		sumaDos->probabilidad = inicioLista->probabilidad;
		sumaDos->izqd = NULL;
		sumaDos->drch = inicioLista;
		sumaDos->continua = NULL;
		inicioLista = sumaDos;

	}

	while(inicioLista->continua != NULL){
		//printf("Estoy en primer while inicioLista->continua != NULL\n");
		struct Hoja* sumaDos = (struct Hoja*)malloc(1*sizeof(struct Hoja));
		sumaDos->probabilidad = inicioLista->probabilidad + inicioLista->continua->probabilidad;
		sumaDos->izqd = inicioLista;
		sumaDos->drch = inicioLista->continua;
		sumaDos->continua = NULL;

		//INSERTAMOS AL FINAL?
		//SI

		if(sumaDos->probabilidad >= finalLista->probabilidad){
			if(inicioLista->continua->continua == NULL){
				inicioLista = sumaDos;
			}else{
				//printf("SI\n");
				finalLista->continua = sumaDos;
				finalLista = sumaDos;
				inicioLista = inicioLista->continua->continua;
			}
			//NO, pues a recorrer la lista.
		}else{
			//printf("NO\n");
			recorreLista = inicioLista->continua->continua;
			if(recorreLista->continua == NULL){
				sumaDos->continua = recorreLista;
				inicioLista = sumaDos;
			}else{

				while(sumaDos->probabilidad >= recorreLista->continua->probabilidad){
					//printf("Buscando lugar\n");
					recorreLista = recorreLista->continua;
				}
				sumaDos->continua = recorreLista->continua;
				recorreLista->continua = sumaDos;
				inicioLista = inicioLista->continua->continua;

			}
		}

	}
	(*raizHuffman) = inicioLista;
}

void recorrerHuffman(struct Hoja* hojaHuffman, char** codigoHuffman,int n, int* contador, double* Lhuffman, double* comprimir, int a){
	int i;
	int borrando = 0;
	if (hojaHuffman != NULL) {
		a = strlen(*codigoHuffman);
		if(n){
			(*codigoHuffman)[a] = '1';
			(*codigoHuffman)[a + 1] = '\0';
		}else{
			(*codigoHuffman)[a] = '0';
			(*codigoHuffman)[a + 1] = '\0';
		}
		if(hojaHuffman->s){
			printf("Numero: %d",*contador);
			*contador = *contador + 1;
			printf("\t Codigo Huffman: ");
			for(i=1; i< strlen(*codigoHuffman);i++){
				printf("%c",(*codigoHuffman)[i]);
			}
			printf("\n");
			borrando = 1;

			*Lhuffman = *Lhuffman + (hojaHuffman->probabilidad*a);
			int x = (hojaHuffman->contador*a);
			*comprimir = *comprimir + x;
		}

		if (borrando){
			(*codigoHuffman)[a] = '\0';
		}
		recorrerHuffman(hojaHuffman->izqd, codigoHuffman, 0, contador, Lhuffman, comprimir, a);
		recorrerHuffman(hojaHuffman->drch, codigoHuffman, 1, contador, Lhuffman, comprimir, a);
		//Borrando
		(*codigoHuffman)[a] = '\0';
	}
}

int main(int argc, char* argv[]){

	int n; //Numero de Bytes por lectura. (Introducido por usuario).
	FILE* fichero; //Fichero que vamos a emplear para lectura.

	if( argc != 3 ){ //Comprobacion de numero de parametros de entrada.

		printf("Error: Numero de parametros de entrada incorrectos.\n");
		exit(1);
	}

	n = atoi(argv[2]); //Obtenemos el numero de Bytes por lectura usando CAST.

	if( n < 1 ){
		printf("Error: Valor de N incorrecto, debe ser mayor que cero.\n");
		exit(1);
	}

	fichero = fopen(argv[1],"rb");

	if ( fichero == NULL ){
		printf("Error: El fichero de entrada no existe o esta dañado.\n");
		exit(1);
	}

	int num_lecturas = 0;
	int num_simbolos = 0; //Numero de simbolos que produce la fuente aleatoria (el archivo). = numero de hojas del arbol

	struct Hoja *inicioLista = NULL;
	struct Hoja *finalLista = NULL;
	struct Hoja *recorreLista = NULL;

	if (n == 1){
		//Variables que necesitamos para el caso n == 1.
		unsigned char * p_lectura;

		// Array de contadores.
		float v_contador [256] = {0};

		// Reserva memoria
		p_lectura = (unsigned char *)malloc(n*sizeof(unsigned char));

		while(!feof(fichero)){
			fread(p_lectura,sizeof(unsigned char),n,fichero);

			int sigue =1;
			if(feof(fichero)){
				if(p_lectura[n] != EOF){
					sigue=0;
				}
			}
			if(sigue){
				num_lecturas++;//Vamos calculando el numero de lecturas totales que vamos a realizar.
				v_contador[(int)*p_lectura]++; //Incrementamos la posicion del vector correspondiente al byte leido.
			}
		}
		crearListaVector(v_contador, &inicioLista, &finalLista, &recorreLista, num_lecturas, n);

	}else if( n > 1){ //Codigo por estructuras.

		struct Hoja *raiz;
		raiz = NULL;

		while(!feof(fichero)){
			//Reserva de memoria
			unsigned char *p_mem = (unsigned char*)malloc(n*sizeof(unsigned char));

			//Lectura fichero de n en n
			fread(p_mem,sizeof(unsigned char),n,fichero);

			int sigue =1;
			if(feof(fichero)){
				if(p_mem[n] != EOF){
					sigue=0;
				}
			}
			if(sigue){
				unsigned char* lec = (unsigned char*)malloc(n*sizeof(unsigned char));
				memcpy(lec,p_mem,n);

				num_lecturas++;

				//Comprobar existencia de nodo con el contenido leido
				if(raiz == NULL){
					//Crear raiz Arbol
					raiz = crear_hoja(p_mem, n);
				}else{
					//LLamada a comparacion
					comparacion(p_mem, raiz, n);
				}
			}
		}
		struct Hoja* RaizProb = NULL;
		struct Hoja* hProb = NULL;

		int* p_cont_hojas = &num_simbolos;
		//printf("Empiezo a crear el arbol ordenado\n");
		recorrerArbol(raiz,p_cont_hojas, num_lecturas,&hProb,&RaizProb,n);
		//printf("He finalizado el arbol ordenado\n");

		//rintf("Comienzo a crear la lista\n");
		crearLista(RaizProb, &inicioLista, &finalLista, &recorreLista, n);
	}

	// Creo el arbol de Huffman
	recorreLista = inicioLista;

	struct Hoja* raizHuffman = NULL;

	crearArbolHuffman(&raizHuffman, inicioLista, finalLista, recorreLista);
	//printf("Acabado de crear el arbol de Huffman\n");
	char* codigoHuffman = (char*)malloc(50*sizeof(char));
	int* contador = (int*)malloc(1*sizeof(int));
	double Lhuffman = 0;
	double comprimir = 0;
	int a = 0;
	*contador = 1;
	recorrerHuffman(raizHuffman,&codigoHuffman, 1, contador, &Lhuffman, &comprimir, a);
	//printf("%lf , %d\n",comprimir,a);

	printf("Longitud Media: %10.9lf\n",Lhuffman);
	double original = num_lecturas*n;
	comprimir = comprimir/8;
	/*if (((int)comprimir%8)!=0){
		comprimir++;
	}
	comprimir = (int)comprimir;*/

	if(comprimir - (int)comprimir > 0){
		comprimir++;
	}

	printf("Bytes de la secuencia comprimida: %d\n",(int)comprimir);
	printf("Bytes del fichero original: %.0lf\n",original);
	printf("Comprimido entre original: %.9lf\n",(int)comprimir/original);

	return(0);
}
