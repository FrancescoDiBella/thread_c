#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define MAX_RAND 2048
int m = 0;//variabile condivisa

pthread_t thread[3];
pthread_mutex_t mutex;
pthread_cond_t cond_var;

int get_random_val(int start_val, int end_val){
    return (rand() %(end_val - start_val +1) + start_val);
}

int get_random_val2(int end_val){
    return get_random_val(0, end_val);
}

void* thread12_routine(void* argv){
    int *thread_numb = (int *)argv;
    char *thread_indentation = "";
    
    if((*(thread_numb) == 1)){
        thread_indentation = "\t\t\t\t\t";
    }

    int m_val;

    printf("%sSono il thread[%d], Sto iniziando!\n", thread_indentation, *thread_numb);
    while(TRUE){

        pthread_mutex_lock(&mutex);
        printf("%sSono il thread[%d], il valore di m: %d\n", thread_indentation, *thread_numb,m);
        m_val = m;
        m = get_random_val2(MAX_RAND);
        
        usleep(300000);

        if((*thread_numb == 0) && (m_val%2 == 0)){
            printf("%sSono il thread[%d], mi metto in attesa\n", thread_indentation, *thread_numb);
            pthread_cond_wait(&cond_var, &mutex);
        }else if((*thread_numb == 1) && (m_val%2 == 1)){
            printf("%sSono il thread[%d], mi metto in attesa\n", thread_indentation, *thread_numb);
            pthread_cond_wait(&cond_var, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }

}

void* thread3_routine(void* argv){
    int thread_numb = 3;
    char *thread_indentation = "\t\t\t\t\t\t\t\t\t\t";

    while(TRUE){

        pthread_mutex_lock(&mutex);
        printf("%sSono il thread[3], entro in azione!\n", thread_indentation);
        pthread_cond_signal(&cond_var);
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

}


int main(int argc, char **argv){
    srand(time(NULL)); 

    printf("Processo inziato \n");
    int p0_code = 0;
    int p1_code = 1;

    pthread_create(&thread[0], NULL, &thread12_routine, (void *)&p0_code);
    pthread_create(&thread[1], NULL, &thread12_routine, (void *)&p1_code);
    pthread_create(&thread[2], NULL, &thread3_routine, NULL);


    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    return 0;
}
