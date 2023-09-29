#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
int hamburguesas_comidas[NUMBER_OF_THREADS] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turno = 0;

void *comer_hamburguesa(void *tid) {
    int thread_id = (int)tid;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (thread_id != turno) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (cantidad_restante_hamburguesas > 0) {
            printf("Hola! Soy el hilo(comensal) %d, me voy a comer una hamburguesa! Quedan %d hamburguesas.\n", thread_id, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--;
            hamburguesas_comidas[thread_id]++;
        } else {
            printf("SE TERMINARON LAS HAMBURGUESAS :( \n");
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        turno = 1 - turno; // Cambiar el turno
        pthread_cond_broadcast(&cond); // Despertar a todos los hilos en espera
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d\n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)i);
        if (status != 0) {
            printf("Algo salió mal al crear el hilo. Código de error: %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hilo %d comió %d hamburguesas.\n", i, hamburguesas_comidas[i]);
    }

    pthread_exit(NULL);
}


