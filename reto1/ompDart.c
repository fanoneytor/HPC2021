#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#define MAX_THREADS 2

long long suma = 0;

void monte_carlo(long long );

int main(int argc, char *argv[]){
    srand(time(NULL));
    long long iteraciones = atoll(argv[1]);
    double pi;

    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    monte_carlo(iteraciones);
    pi = 4.0*suma/(double) iteraciones;
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    
    printf("Pi: %f\n",pi);
    printf("Time measured: %.3f seconds.\n", elapsed);

    return 0;
}

void monte_carlo(long long iteraciones){
    long long i;
    double x, y, d;
    #pragma omp parallel for num_threads(MAX_THREADS) private(x, y, d) shared(suma) schedule(static)
    for(long long i=0; i<iteraciones; i++){
        //Genera una coordenada (x,y)
        x = (double)rand()/(double)(RAND_MAX/2);
        x = x - 1;
        y = (double)rand()/(double)(RAND_MAX/2);
        y = y - 1;
        //Calcula si el dardo cae en el circulo
        d = (x*x) + (y*y);
        if(d<=1){
            suma = suma+1;
        }
    }
    #pragma omp barrier
}