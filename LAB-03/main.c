#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 12
#define NUM_ORIGINS 4
#define NUM_DESTINATIONS 4
#define TOTAL_THREADS (NUM_ORIGINS + NUM_DESTINATIONS + 1) // 4 origens, 4 destinos, 1 mailbox

int canal[NUM_DESTINATIONS] = {-1, -1, -1, -1};

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Mailbox;

Mailbox mailbox;

void init_mailbox(Mailbox *mailbox) {
    mailbox->count = 0;
    pthread_mutex_init(&mailbox->lock, NULL);
    pthread_cond_init(&mailbox->not_full, NULL);
    pthread_cond_init(&mailbox->not_empty, NULL);
}

void send_mail(Mailbox *mailbox, int message) {
    pthread_mutex_lock(&mailbox->lock);
    while (mailbox->count == BUFFER_SIZE) {
        pthread_cond_wait(&mailbox->not_full, &mailbox->lock);
    }
    mailbox->buffer[mailbox->count++] = message;
    pthread_cond_signal(&mailbox->not_empty);
    pthread_mutex_unlock(&mailbox->lock);
}

int receive_mail(Mailbox *mailbox) {
    int message;
    pthread_mutex_lock(&mailbox->lock);
    while (mailbox->count == 0) {
        pthread_cond_wait(&mailbox->not_empty, &mailbox->lock);
    }
    message = mailbox->buffer[--mailbox->count];
    pthread_cond_signal(&mailbox->not_full);
    pthread_mutex_unlock(&mailbox->lock);
    return message;
}

int converte_valor(unsigned char bytex2, unsigned char bytex1) {
    return (bytex2 << 8) | bytex1;
}

void encontra_bytes(int valor, unsigned char *byte2, unsigned char *byte1) {
    *byte2 = (valor >> 8) & 255;
    *byte1 = valor & 255;
}

void send_async(int *buf, int c) {
    canal[c] = *buf;
}

void receive(int *buf, int c) {
    while (canal[c] == -1);
    *buf = canal[c];
    canal[c] = -1;
}

void *origin_thread(void *arg) {
    int thread_id = *(int *)arg;
    unsigned char dest = thread_id;
    unsigned char value = (thread_id + 1) * 10;
    int message = converte_valor(dest, value);
    printf("Thread %c enviando mensagem: Dest=%c, Valor=%d\n", 'A' + thread_id, 'E' + thread_id, value);
    send_mail(&mailbox, message);
    return NULL;
}

void *destination_thread(void *arg) {
    int thread_id = *(int *)arg;
    unsigned char dest, value;
    while (1) {
        int message;
        receive(&message, thread_id);
        encontra_bytes(message, &dest, &value);
        printf("Thread %c recebeu mensagem: Valor=%d\n", 'E' + thread_id, value);
        sleep(1);
    }
    return NULL;
}

void *mailbox_thread(void *arg) {
    while (1) {
        int message = receive_mail(&mailbox);
        unsigned char dest, value;
        encontra_bytes(message, &dest, &value);
        send_async(&message, dest);
    }
    return NULL;
}

int main() {
    pthread_t threads[TOTAL_THREADS];
    init_mailbox(&mailbox);
    int thread_ids[NUM_ORIGINS];
    
    pthread_create(&threads[NUM_ORIGINS + NUM_DESTINATIONS], NULL, mailbox_thread, NULL);
    
    for (int i = 0; i < NUM_ORIGINS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, origin_thread, &thread_ids[i]);
    }
    
    for (int i = 0; i < NUM_DESTINATIONS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[NUM_ORIGINS + i], NULL, destination_thread, &thread_ids[i]);
    }
    
    for (int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
