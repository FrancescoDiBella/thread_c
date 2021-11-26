/*
variabile globale int sample a 50

3 thread accedono ciclicamente a sample, i thread creano un int 
Z randomico compreso tra 10 e 90
il thread scrive "Sono il thread X: sample valeva Y, ora vale Z"
se Y == Z il cliclo finisce e il thread ritorna
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int sample = 50;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * fun(void *arg){
    int flag = 1;
    char * thread = (char *) arg;

    while(flag){
        int rndm_value = (rand()%81) + 10; //genera numeri randomici tra 10 e 90, estremi inclusi
        usleep(300000);
        pthread_mutex_lock(&mutex);
        int cpy = sample;
        sample = rndm_value;
        printf("%s: sample valeva %d, ora vale %d\n", thread, cpy, sample);
        pthread_mutex_unlock(&mutex);
        
        if(sample == cpy) {
            flag = 0;
            printf("%s: Ho finito!\n", thread);
        }
    }
    return NULL;
}

int main(){
    pthread_t t1, t2, t3;

    srand(time(NULL));
    
    pthread_create(&t1, NULL, &fun, (void *)"t1");
    pthread_create(&t2, NULL, &fun, (void *)"\t\t\t\t\t\t2");
    pthread_create(&t3, NULL, &fun, (void *)"\t\t\t\t\t\t\t\t\t\t\t\tt3");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Processo padre terminato correttamente!\n");
    return 0;
}
