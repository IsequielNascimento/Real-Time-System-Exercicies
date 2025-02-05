#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_ORIGENS 4
#define NUM_DESTINOS 4
#define BUFFER_SIZE 12
#define TOTAL_MENSAGENS 4

int mailbox[BUFFER_SIZE];
int mensagens[NUM_DESTINOS]; // Array separado para armazenar mensagens entregues
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_mailbox = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_destino[NUM_DESTINOS] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};

char *origens[NUM_ORIGENS] = {"A", "B", "C", "D"};
char *destinos[NUM_DESTINOS] = {"E", "F", "G", "H"};

// Função para compor a mensagem
int converte_valor(unsigned char bytex2, unsigned char bytex1) {
    return (bytex2 << 8) | bytex1;
}

// Função para extrair os bytes da mensagem
void encontra_bytes(int valor, unsigned char *byte2, unsigned char *byte1) {
    *byte2 = (valor >> 8) & 0xFF;
    *byte1 = valor & 0xFF;
}

void *thread_send(void *arg) {
    int id = *((int *)arg);
    unsigned char valores[NUM_ORIGENS] = {10, 20, 30, 40};
    unsigned char destinos_id[NUM_ORIGENS] = {4, 5, 6, 7};
    int frame = converte_valor(destinos_id[id], valores[id]);

    pthread_mutex_lock(&mutex);
    mailbox[count++] = frame;
    printf("Origem %s enviou %d para %s\n", origens[id], valores[id], destinos[id]);
    if (count == TOTAL_MENSAGENS) {
        pthread_cond_signal(&cond_mailbox);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *thread_mailbox(void *arg) {
    pthread_mutex_lock(&mutex);
    while (count < TOTAL_MENSAGENS) {
        pthread_cond_wait(&cond_mailbox, &mutex);
    }
    
    for (int i = 0; i < TOTAL_MENSAGENS; i++) {
        unsigned char destino, valor;
        encontra_bytes(mailbox[i], &destino, &valor);
        int index = destino - 4;
        mensagens[index] = valor; // Armazena no array correto
        pthread_cond_signal(&cond_destino[index]);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_receive(void *arg) {
    int id = *((int *)arg);
    pthread_mutex_lock(&mutex);
    while (mensagens[id] == -1) {
        pthread_cond_wait(&cond_destino[id], &mutex);
    }
    printf("Thread %s recebeu: %d\n", destinos[id], mensagens[id]);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void) {
    pthread_t senders[NUM_ORIGENS], receivers[NUM_DESTINOS], mailbox_thread;
    int ids[NUM_ORIGENS] = {0, 1, 2, 3};
    
    for (int i = 0; i < BUFFER_SIZE; i++) {
        mailbox[i] = -1;
    }
    for (int i = 0; i < NUM_DESTINOS; i++) {
        mensagens[i] = -1;
    }
    
    pthread_create(&mailbox_thread, NULL, thread_mailbox, NULL);
    
    for (int i = 0; i < NUM_ORIGENS; i++) {
        pthread_create(&senders[i], NULL, thread_send, &ids[i]);
    }
    for (int i = 0; i < NUM_DESTINOS; i++) {
        pthread_create(&receivers[i], NULL, thread_receive, &ids[i]);
    }
    
    for (int i = 0; i < NUM_ORIGENS; i++) {
        pthread_join(senders[i], NULL);
    }
    pthread_join(mailbox_thread, NULL);
    for (int i = 0; i < NUM_DESTINOS; i++) {
        pthread_join(receivers[i], NULL);
    }
    
    return 0;
}
