#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//Funciones prototipo
int monte_carlo(double,long long );


#ifndef MPI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char *argv[]){
    double l = atof(argv[1]);
    long long iteraciones = atoll(argv[2]); 
    double p, pi;
    long long n_cruzados;
    
    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    n_cruzados = monte_carlo(l,iteraciones);
    p = n_cruzados/(double) iteraciones;
    //Infiere Pi desde la probabilidad, sabiendo P = 2*longitud/Pi*Distancia
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

int monte_carlo(double l, long long iteraciones){
    double x, theta, n;
    long long n_cruzados = 0;
    for(long long i=0; i<iteraciones; i++){
        //Genera el estado (X, Theta)
        x = (double)rand()/(double)(RAND_MAX/l);
        theta = (double)rand()/(double)(RAND_MAX/M_PI);

        //Calcula si la aguja cruza
        n = sin(theta)/2.0;
        if(x+n >= l || x-n<= 0){
            n_cruzados++;
        }
    }
    return n_cruzados;
}