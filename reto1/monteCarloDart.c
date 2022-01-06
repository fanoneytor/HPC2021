#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Funciones prototipo
long long monte_carlo(long long);




int main(int argc, char *argv[]){
    long long iteraciones = atoll(argv[1]);
    double pi;
    long long p_circulo=0;
    
    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    p_circulo = monte_carlo(iteraciones);
    //Calcula el valor de Pi
    pi = 4.0*p_circulo/(double) iteraciones;
    
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    
    printf("Pi: %f\n",pi);
    printf("Time measured: %.3f seconds.\n", elapsed); 

    return 0;
}

long long monte_carlo(long long iteraciones){
    double x, y, d;
    long long p_circulo=0;

    for(long long i=0; i<iteraciones; i++){
        //Genera una coordenada (x,y)
        x = (double)rand()/(double)(RAND_MAX/2);
        x = x - 1;
        y = (double)rand()/(double)(RAND_MAX/2);
        y = y - 1;
        //Calcula si el dardo cae en el circulo
        d = (x*x) + (y*y);
        if(d<=1){
            p_circulo++;
        }
        
    }
    return p_circulo;
}