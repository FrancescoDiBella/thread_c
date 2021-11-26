#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define RANGE_RAND 11 //numero compreso tra 0 e 1
#define MILLIS_FACTOR 1000

int x;
pthread_mutex_t mutex;
pthread_cond_t differenza_magg;

void * routine_T1(void * argv){
    int m;
    while(1){
        usleep(100*MILLIS_FACTOR); //si addormenta per 100ms

        m = rand()%RANGE_RAND; // si genera un numero casuale tra 0 e 10 estremi inclusi

        pthread_mutex_lock(&mutex); //inizio regione critica, lock su mutex
        if(x == -1){
            pthread_mutex_unlock(&mutex); //unlock prima di terminare l'esecuzione per evitare deadlock su altri thread
            return NULL;
        }
        else{
            if(x == m) {
                printf("RISPOSTA CORRETTA\n");
                x = -1;
                pthread_mutex_unlock(&mutex); //unlock prima di terminare l'esecuzione per evitare deadlock su altri thread
                return NULL;
            }
            if(abs(m-x) > 5){
                printf("RISPOSTA MOLTO SBAGLIATA\n");
                pthread_cond_wait(&differenza_magg, &mutex); //il thread T1 si blocca su una variabile condizione
            }else{
                printf("RISPOSTA SBAGLIATA\n");
            }
        }
        pthread_mutex_unlock(&mutex); //fine regione critica

    }

    return NULL;
}

void * routine_T2(void * argv){
    while(1){
        usleep(300*MILLIS_FACTOR); //si addormenta per 300ms

        pthread_mutex_lock(&mutex); //inizio regione critica
        pthread_cond_signal(&differenza_magg); //signal sulla coda dei thread in wait su quella particolare varibile di condizione
        if(x == -1){
            pthread_mutex_unlock(&mutex); //unlock prima di terminare l'esecuzione per evitare deadlock su altri thread
            return NULL;
        }
        pthread_mutex_unlock(&mutex); //fine regione critica
    }

    return NULL;
}

int main(){
    srand(time(NULL));
    pthread_t T1, T2;

    x = rand()%RANGE_RAND;

    pthread_create(&T1, NULL, routine_T1, NULL);
    pthread_create(&T2, NULL, routine_T2, NULL);

    pthread_join(T1, NULL);
    pthread_join(T2, NULL);
    printf("Processo padre concluso normalmente\n");

    return 0;
}
