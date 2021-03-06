#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>



long long monte_carlo(pid_t*,double*, double, long long );
void needles(double*, double, long long, long long, int);

#define MAX_PROCESS 8
#ifndef MPI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char *argv[]){
    double l = atof(argv[1]);
    long long iteraciones = atoll(argv[2]); 
    double p, pi;
    long long n_cruzados;
    double *sumas = NULL;
    sumas = (double*)mmap(NULL,sizeof(double*)*MAX_PROCESS,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
    
    pid_t *forks = (pid_t*) malloc(MAX_PROCESS*sizeof(pid_t));
    
    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    n_cruzados = monte_carlo(forks,sumas,l,iteraciones);
    p = n_cruzados/(double) iteraciones;
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

long long monte_carlo(pid_t * forks,double * sumas, double l, long long iteraciones){
    long long suma=0;
    long long iter_per_process = iteraciones/MAX_PROCESS;
    int status = 0;
    for(int i=0;i<MAX_PROCESS;i++){
        if((forks[i]=fork()) < 0){
            perror("fork");
            abort();
        }
        else if(forks[i] == 0){
            needles(sumas, l, iteraciones,iter_per_process,i);
            exit(0);
        }
    }
    
    for(int i=0; i<MAX_PROCESS; i++){
        wait(NULL);
    }
    
    for(int i=0;i<MAX_PROCESS;i++){
        suma = suma + sumas[i];
    }
    return suma;
}

void needles(double*sumas, double l, long long iteraciones,long long iter_per_process,int proceso){
    double x, theta, n;
    sumas[proceso] = 0;
    for(long long i=0; i<iter_per_process; i++){
        //Genera el estado (X, Theta)
        x = (double)rand()/(double)(RAND_MAX/l);
        theta = (double)rand()/(double)(RAND_MAX/M_PI);

        //Calcula si la aguja cruza
        n = sin(theta)/2.0;
        if(x+n >= l || x-n<= 0){
            sumas[proceso] = sumas[proceso] + 1;
        }
    }
    if(proceso == 0){
        double resto =iteraciones%MAX_PROCESS;
        for(int i=0;i<resto;i++){
            x = (double)rand()/(double)(RAND_MAX/l);
            theta = (double)rand()/(double)(RAND_MAX/M_PI);

            //Calcula si la aguja cruza
            n = sin(theta)/2.0;
            if(x+n >= l || x-n<= 0){
                sumas[proceso] = sumas[proceso] + 1;
            }
        }
    }
    
}