#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define T0_OP 0
#define T1_OP 1
#define THREAD_0_MAX 10 //DA CHIAMARE LIMIT
#define THREAD_1_MAX -10

pthread_mutex_t mutex;
int pos = 0;
int vittorie_t0 = 0;
int vittorie_t1 = 0;
pthread_cond_t vittoria_t;

int get_base_value(int* value, int thread_op){
    int pos_t = -(*value);

    return 0;

}

void * game_routine(int * thread_id){
    int force_sign = 0;
    int thread_limit;

    if(*thread_id == T0_OP){
        force_sign = -1;
        thread_limit = THREAD_0_MAX;
    }else if(*thread_id == T1_OP){
        force_sign = 1;
        thread_limit = THREAD_1_MAX;
    }else return NULL;

    while(1){
        int recupero = rand()%4;
        int forza = rand()%6;
        printf("Sono il thread [%d] e mi addormenterò per %ds .z..z...Z\n", *thread_id, recupero);

        sleep(recupero);

        pthread_mutex_lock(&mutex);

        if(vittorie_t0 >= 10 || vittorie_t1 >= 10){
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        if(*thread_id == T0_OP){ //thread0
            if(pos >=10){ //vince l'altro thread
                vittorie_t1++;
                pos = 0;
                pthread_cond_signal(&vittoria_t);
            }else{
                pos -=forza;
                if(pos <= -10){
                    pthread_cond_wait(&vittoria_t, &mutex);
                    printf("Ho vinto! -p0\n");
                }
            }
        }else{
            if(pos <= -10){
                vittorie_t0++;
                pos = 0;
                pthread_cond_signal(&vittoria_t);
            }else{
                pos +=forza;
                if(pos >= 10){
                    pthread_cond_wait(&vittoria_t, &mutex);
                    printf("Ho vinto! -p1\n");
                }  
            }
        }


        pthread_mutex_unlock(&mutex);

    }

}

int main(){
    srand(time(NULL));
    pthread_t t0,t1;
    int t0_id = 0;
    int t1_id = 1;


    pthread_create(&t0, NULL, (void *)game_routine, (void *) &t0_id);
    pthread_create(&t1, NULL, (void *)game_routine, (void *) &t1_id);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    if(vittorie_t0 > vittorie_t1){
        printf("Il thread t0 ha vinto!\n");
    }else{
        printf("Il thread t1 ha vinto!\n");
    }
    printf("Il processo padre è finito correttamente!\n");
    return 0;
}