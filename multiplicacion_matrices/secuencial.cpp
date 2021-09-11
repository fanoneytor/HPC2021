#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **punteroMatrizA, **punteroMatrizB, **punteroResultado;

void llenarMatrices(int n);
void mostrarMatrices(int n);
void productoMatrices(int n);

int main(int argc, char *argv[]){
	int n;
	//printf("Ingrese la dimension de la matriz: ");
	//n = atoi(argv[1]);
    n = atoi(argv[1]); //con argv recibo el numero desde consola linux.
	
	punteroMatrizA = new int*[n];
	punteroMatrizB = new int*[n];
	punteroResultado = new int*[n];
	for(int i=0; i<n; i++){
		punteroMatrizA[i] = new int[n];
		punteroMatrizB[i] = new int[n];
		punteroResultado[i] = new int[n];
	}		
	
	llenarMatrices(n);

    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

	productoMatrices(n);
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    
    printf("Time measured: %.3f seconds.\n", elapsed); 
	//mostrarMatrices(n);
	
	
	for(int i=0; i<n; i++){
		delete[] punteroMatrizA[i];
		delete[] punteroMatrizB[i];
		delete[] punteroResultado[i];
		
	}
	delete[] punteroMatrizA;
	delete[] punteroMatrizB;
	delete[] punteroResultado;
}

void llenarMatrices(int n){
	
	srand (time(NULL));
	int numeroA, numeroB;	
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			numeroA = rand() % 10 + 1;
			numeroB = rand() % 10 + 1;
			*(*(punteroMatrizA+i)+j)=numeroA;
			*(*(punteroMatrizB+i)+j)=numeroB;
		}
	}	
}

void mostrarMatrices(int n){
	printf("Matriz A:\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			printf("%d	", *(*(punteroMatrizA+i)+j));
		}
		printf("\n");
	}
	
	printf("Matriz B:\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			printf("%d	", *(*(punteroMatrizB+i)+j));
		}
		printf("\n");
	}
	
	printf("RESULTADO:\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			printf("%d	", *(*(punteroResultado+i)+j));
		}
		printf("\n");
	}
}

void productoMatrices(int n){
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			int producto = 0;
			for(int k=0; k<n; k++){
				producto += *(*(punteroMatrizA+i)+k) * *(*(punteroMatrizB+k)+j);
			}
			*(*(punteroResultado+i)+j) = producto;
		}
	}
}