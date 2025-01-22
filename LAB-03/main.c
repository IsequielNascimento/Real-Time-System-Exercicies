#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 12
#define NUM_THREADS 8

// Estrutura da mailbox
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Mailbox;

// Inicializa a mailbox
void init_mailbox(Mailbox *mailbox) {
    mailbox->count = 0;
    pthread_mutex_init(&mailbox->lock, NULL);
    pthread_cond_init(&mailbox->not_full, NULL);
    pthread_cond_init(&mailbox->not_empty, NULL);
}

// Insere uma mensagem na mailbox
void send_mail(Mailbox *mailbox, int message) {
    pthread_mutex_lock(&mailbox->lock);
    while (mailbox->count == BUFFER_SIZE) {
        pthread_cond_wait(&mailbox->not_full, &mailbox->lock);
    }
    mailbox->buffer[mailbox->count++] = message;
    pthread_cond_signal(&mailbox->not_empty);
    pthread_mutex_unlock(&mailbox->lock);
}

// Remove uma mensagem da mailbox
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

// Função para construir a mensagem com identificador de destino e valor
int build_message(unsigned char dest, unsigned char value) {
    return (dest << 8) | value;
}

// Função para extrair o identificador de destino e valor da mensagem
void parse_message(int message, unsigned char *dest, unsigned char *value) {
    *dest = (message >> 8) & 0xFF;
    *value = message & 0xFF;
}

// Threads de origem
void *origin_thread(void *arg) {
    Mailbox *mailbox = (Mailbox *)arg;
    int thread_id = (int)(size_t)arg;
    unsigned char dest = thread_id + 4; // Destinos E, F, G, H
    unsigned char value = (thread_id + 1) * 10;
    int message = build_message(dest, value);

    printf("Thread A%d enviando mensagem: Dest=%c, Valor=%d\n", thread_id, dest + 'E' - 4, value);
    send_mail(mailbox, message);

    return NULL;
}

// Threads de destino
void *destination_thread(void *arg) {
    Mailbox *mailbox = (Mailbox *)arg;
    unsigned char dest, value;

    while (1) {
        int message = receive_mail(mailbox);
        parse_message(message, &dest, &value);
        printf("Thread %c recebeu mensagem: Valor=%d\n", dest + 'E', value);
        sleep(1); // Simula processamento
    }
    return NULL;
}

// Thread da mailbox
void *mailbox_thread(void *arg) {
    Mailbox *mailbox = (Mailbox *)arg;
    int messages[BUFFER_SIZE];
    int i;

    while (1) {
        for (i = 0; i < BUFFER_SIZE; ++i) {
            messages[i] = receive_mail(mailbox);
        }
        for (i = 0; i < BUFFER_SIZE; ++i) {
            send_mail(mailbox, messages[i]);
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    Mailbox mailbox;
    init_mailbox(&mailbox);

    // Criar threads de origem (A, B, C, D)
    for (int i = 0; i < 4; ++i) {
        pthread_create(&threads[i], NULL, origin_thread, (void *)(size_t)i);
    }

    // Criar threads de destino (E, F, G, H)
    for (int i = 4; i < 8; ++i) {
        pthread_create(&threads[i], NULL, destination_thread, (void *)&mailbox);
    }

    // Aguarda todas as threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
