#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];

sem_t empty;
sem_t full;
sem_t mutex;
int item = 1;

void *producer(void *arg) {
   
    int loops = *((int *)arg);
    for (int i = 0; i < loops; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[item % BUFFER_SIZE] = item;
        printf("Produced item: %d\n", item);
        sem_post(&mutex);
        sem_post(&full);
        item++;
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int loops = *((int *)arg);
    for (int i = 0; i < loops; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = buffer[item % BUFFER_SIZE];
        printf("Consumed item: %d\n", item);
        sem_post(&mutex);
        sem_post(&empty);
        item--;
    }
    pthread_exit(NULL);
}

int main() {
	
    int loops;
    printf("enter the number items produced :\n");
    scanf("%d",&loops);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t producerThread, consumerThread;
    
    pthread_create(&producerThread, NULL, producer, &loops);
    pthread_create(&consumerThread, NULL, consumer, &loops);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
