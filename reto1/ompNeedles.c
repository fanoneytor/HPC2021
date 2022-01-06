#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#ifndef MPI
#define M_PI 3.14159265358979323846
#endif
#define MAX_THREADS 8

long long suma = 0;

void monte_carlo(double, long long);

int main(int argc, char *argv[]){
    srand(time(NULL));
    double l = atof(argv[1]);
    long long iteraciones = atoll(argv[2]); 
    double p, pi;

    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    monte_carlo(l,iteraciones);
    p = suma/(double) iteraciones;
    pi = 2.0/(p*l);

    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    
    printf("Probabilidad: %f\n",p);
    printf("Pi: %f\n", pi);
    printf("Time measured: %.3f seconds.\n", elapsed);

    return 0;
}

void monte_carlo(double l, long long iteraciones){
    long long i;
    double x, theta, n;
    long long rep = 0;
    #pragma omp parallel for
    for(i=0; i<iteraciones; i++){
        //Genera el estado (X, Theta)
        rep ++;
        x = (double)rand()/(double)(RAND_MAX/l);
        theta = (double)rand()/(double)(RAND_MAX/M_PI);

        //Calcula si la aguja cruza
        n = sin(theta)/2.0;
        if(x+n >= l || x-n<= 0){
            suma = suma+1;    
        }
    }
    #pragma omp barrier
}