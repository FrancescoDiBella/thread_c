/*
 * Una variabile intera n, inizializzata a 0, è condivisa tra 2 thread tO, tE.
 * Il thread tE, ciclicamente:
 * 	1. attende 200 ms (N.B.: la chiamata usleep(t) attende per t microsecondi)
 * 	2. genera un int casuale pari e lo somma alla variabile condivisa n
 * 	3. se ha eseguito almeno 10 cicli e n è pari termina
 * 	4. altrimenti ricomincia da (1), a meno che abbia già compiuto 1000 iterazioni, nel qual caso termina.
 * Il thread tO, ciclicamente:
 * 	1. attende 200 ms (N.B.: la chiamata usleep(n) attende per n microsecondi)
 * 	2. genera un int casuale dispari e lo somma alla variabile condivisa n
 * 	3. se ha eseguito almeno 10 cicli e n è dispari termina
 * 	4. altrimenti ricomincia da (1), a meno che abbia già compiuto 1000 iterazioni, nel qual caso termina.

 * (Non ricorrere a un array di 2 thread per l’implementazione!)

 * Il programma termina quando tutti i thread hanno terminato la propria esecuzione. 
 I thread scriveranno di essere terminati. Possono anche visualizzare, a ogni ciclo, 
 il valore trovato in n.
 * Nel codice, proteggere opportunamente la variabile n dagli accessi concorrenti. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define MAX_RAND 101

int n = 0;
pthread_mutex_t mutex;

void * thread_routine(char *thread_type){
    int count = 0;

    int should_generate_even_numb = strcmp(thread_type, "tE") == 0;

    int factor = should_generate_even_numb ? 0 : 1;


    while(count < 1000){
        usleep(200000);
        int rand_val = (rand()%MAX_RAND)* 2 + factor;

        pthread_mutex_lock(&mutex);
        n+=rand_val;
        printf("Sono il thread %s, sono all'iterazione n. %d: valore di n >>> %d e ho generato il numero random>>> %d\n", thread_type, count+1,n, rand_val);
        if(count>=9 && (n%2)==factor){
            pthread_mutex_unlock(&mutex);
            printf("Sono il thread %s: Sto terminando\n", thread_type);
            break;
        }
        pthread_mutex_unlock(&mutex);
        count++;
    }
    return NULL;
}

int main(){
    srand(time(NULL));
    pthread_t tO, tE;

    pthread_create(&tE, NULL, (void *)thread_routine, (void *)"tE");
    pthread_create(&tO, NULL, (void *)thread_routine, (void *)"tO");


    pthread_join(tE, NULL);
    pthread_join(tO, NULL);

    printf("Processo padre terminato correttamente!\n");
    return 0;
}