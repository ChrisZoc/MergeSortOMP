#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

#define MAX_SIZE 100000
int omp_thread_count() {
    int n = 0;
    #pragma omp parallel reduction(+:n)
    n += 1;
    return n;
}

void leer_archivo(int * x, int n) {

	FILE *myFile;
	myFile = fopen("100k.txt", "r");

	int i;

	if (myFile == NULL)
	{
        	printf("Error Leyendo Archivo\n");
        	exit (0);
	}
	for (i = 0; i < n; i++)
	{
        	fscanf(myFile, "%d,", &x[i] );
	}

	printf("Archivo leido\n");

    	for (i = 0; i < 16; i++)
   	{
		printf("Number is: %d\n\n", x[i]);
        }
}

void imprimir_array(int * x, int n) {
   int i;
   for (i = 0; i < n; i++) {
      printf("%d ",x[i]);
   } 
}

void merge(int *aux, int *v, int i, int m, int f) {
	int j;
	int k;
	for(j=i; j<=m; j++)
		aux[j-i] = v[j];
	
	int c1=0, c2=m+1;
	for(j=i; j<=f; j++) {
		if(aux[c1] < v[c2]) {
			v[j] = aux[c1++];
			if(c1==m-i+1)
				for(k=c2; k<=f; k++)
					v[++j] = v[k];
		}
		else {
			v[j] = v[c2++];
			if(c2==f+1)
				for(k=c1; k<=m-i; k++)
					v[++j] = aux[k];
		}
	}
	
}
void sort(int *tmp, int *num,int low, int high) {
int mid;

if(low < high) {
      mid = (low + high) / 2;
      sort(tmp, num, low, mid);
      sort(tmp, num, mid+1, high);
     merge(tmp, num, low, mid, high);
   } else { 
      return;
   }   
}

void sendSort(int *tmp, int *num,int n,int p){

int id = omp_get_thread_num();
if(id==0){
sort(tmp, num, (id)*(n/p) ,(id+1)*(n/p)-1);
//printf("desde0: %d\nhasta0: %d\n",(id)*(n/p) ,(id+1)*(n/p)-1);
}if(id==(omp_thread_count()-1)){
sort(tmp, num, ((id)*(n/p)),n-1);
}else{
sort(tmp, num, ((id)*(n/p)),(id+1)*(n/p)-1);
//printf("desde: %d\nhasta: %d\n",((id)*(n/p))+1 ,((id+1)*(n/p))-1);
}


}
int main()
{
   int n = 100000;
   double inicio, fin, llenar1, llenar2;
   int p = omp_thread_count();  
   int numeros[MAX_SIZE], tmp[MAX_SIZE];
   printf("n es: %d\n",n);
   printf("p es: %d\n",p);
   leer_archivo(numeros, n);
   //printf("Lista antes de ordenar...\n");
   //imprimir_array(numeros, n);
inicio = omp_get_wtime();
   #pragma omp parallel 
   sendSort(tmp,numeros,n, p);   

   
fin = omp_get_wtime();
   //printf("\nLista despues de ordenar...\n");
   //imprimir_array(numeros, n);
   printf("\nTiempo: %g\n",fin-inicio);
}
