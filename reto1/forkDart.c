#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>

long long monte_carlo(pid_t*, double*, long long);
void darts(double*,long long, long long, int);

#define MAX_PROCESS 2

int main(int argc, char *argv[]){
    long long iteraciones = atoll(argv[1]);
    double pi;
    long long p_circulo=0;
    double *sumas = NULL;
    sumas = (double*)mmap(NULL,sizeof(double*)*MAX_PROCESS,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
    
    pid_t *forks = (pid_t*) malloc(MAX_PROCESS*sizeof(pid_t));
    
    // Start measuring time
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    p_circulo = monte_carlo(forks,sumas,iteraciones);
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

long long monte_carlo(pid_t * forks,double * sumas, long long iteraciones){
    long long suma=0;
    long long iter_per_process = iteraciones/MAX_PROCESS;
    int status = 0;
    for(int i=0;i<MAX_PROCESS;i++){
        if((forks[i]=fork()) < 0){
            perror("fork");
            abort();
        }
        else if(forks[i] == 0){
            darts(sumas,iteraciones,iter_per_process,i);
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

void darts(double*sumas, long long iteraciones,long long iter_per_process,int proceso){
    double x, y, d;
    sumas[proceso] = 0;
    for(long long i=0; i<iter_per_process; i++){
        //Genera una coordenada (x,y)
        x = (double)rand()/(double)(RAND_MAX/2);
        x = x - 1;
        y = (double)rand()/(double)(RAND_MAX/2);
        y = y - 1;
        //Calcula si el dardo cae en el circulo
        d = (x*x) + (y*y);
        if(d<=1){
            sumas[proceso] = sumas[proceso] + 1;
        }
    }

    if(proceso==0){
        double resto =iteraciones%MAX_PROCESS;
        for(int i=0;i<resto;i++){
            x = (double)rand()/(double)(RAND_MAX/2);
            x = x - 1;
            y = (double)rand()/(double)(RAND_MAX/2);
            y = y - 1;
            //Calcula si el dardo cae en el circulo
            d = (x*x) + (y*y);
            if(d<=1){
                sumas[proceso] = sumas[proceso] + 1;
            }
        }
    }
}