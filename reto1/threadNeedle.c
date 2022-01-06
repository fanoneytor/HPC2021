#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <error.h>
#include <pthread.h>




#ifndef MPI
#define M_PI 3.14159265358979323846
#endif
#define MAX_THREADS 2

pthread_mutex_t mutex;
long long suma = 0;
typedef struct{
    long long iteraciones;
    long long total_iteraciones;
    double l;
    int thread;
}estructura_datos;

//Funciones prototipo
void monte_carlo(estructura_datos*,pthread_t*,double,long long );
void *needles(void*estructura_datos);

int main(int argc, char *argv[]){
    srand(time(NULL));
    double l = atof(argv[1]);
    long long iteraciones = atoll(argv[2]); 
    double p, pi;
    estructura_datos * datos = NULL;
    pthread_t *hilos = (pthread_t*) malloc(MAX_THREADS*sizeof(pthread_t));
    

    //Calcula la probabilidad de que las agujas se crucen
    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    monte_carlo(datos,hilos,l,iteraciones);
    //printf("cruce: %lld",n_cruzados);
    p = suma/(double) iteraciones;
    //Infiere Pi desde la probabilidad, sabiendo P = 2*longitud/Pi*Distancia
    pi = 2.0/(p*l);

    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("Probabilidad: %f\n",p);
    printf("Pi: %f\n",pi);
    printf("Time measured: %.3f seconds.\n", elapsed);

    return 0;
}

void monte_carlo(estructura_datos * datos,pthread_t * hilos, double l, long long iteraciones){
    

    pthread_mutex_init(&mutex,NULL);
    for(int i=0;i<MAX_THREADS;i++){
        datos = (estructura_datos *) malloc(sizeof(estructura_datos));
        datos->iteraciones=iteraciones/MAX_THREADS;
        datos->total_iteraciones = iteraciones;
        datos->l=l;
        datos->thread = i;
        
        pthread_create(&(hilos[i]),NULL,needles,(void *) datos);
    }
    for(int i=0;i<MAX_THREADS;i++){
        pthread_join(hilos[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    
}

void * needles(void * estructura){
    estructura_datos *datos = (estructura_datos *) estructura;
    double x, theta, n;
    long long iteraciones = datos->iteraciones;
    double l = datos->l;
    for(long long i=0; i<iteraciones; i++){
        //Genera el estado (X, Theta)
        
        x = (double)rand()/(double)(RAND_MAX/l);
        theta = (double)rand()/(double)(RAND_MAX/M_PI);

        //Calcula si la aguja cruza
        n = sin(theta)/2.0;
        if(x+n >= l || x-n<= 0){
            pthread_mutex_lock(&mutex);
            suma = suma+1;
            pthread_mutex_unlock(&mutex);
        }
    }
    if(datos->thread==0){
        double resto = datos->total_iteraciones%MAX_THREADS;

        for(int i=0;i<resto;i++){
            x = (double)rand()/(double)(RAND_MAX/l);
            theta = (double)rand()/(double)(RAND_MAX/M_PI);

            //Calcula si la aguja cruza
            n = sin(theta)/2.0;
            if(x+n >= l || x-n<= 0){
                pthread_mutex_lock(&mutex);
                suma = suma++;
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    
    return NULL;

}