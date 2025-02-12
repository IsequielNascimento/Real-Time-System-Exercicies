#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER 12

int canal[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

void send_sync(int *buf, int c) {
    canal[c] = *buf;
    while (canal[c] != -1);
}

void receive(int *buf, int c) {
    while (canal[c] == -1); 
    *buf = canal[c];
    canal[c] = -1;
}

// Constrói frame com dois bytes, o MSB e o LSB 
int converte_valor(unsigned char bytex2, unsigned char bytex1){              
  return bytex2*256 + bytex1;
}

// Extrai os dois bytes da word passada, o MSB e o LSB 
void encontra_bytes(int valor, unsigned char * byte2, unsigned char * byte1){
  *byte2 = ((valor >> 8) & 255);
  *byte1 = (valor & 255);
  return;
}   

// Threads de origem (enviando frames)
void *TA(void *threadno) {
    unsigned char ID = 4;  
    unsigned char valor = 10;
    int frame = converte_valor(ID, valor);
    while (1) {
        send_sync(&frame, 0);
       
    }
    return NULL;
}

void *TB(void *threadno) {
    unsigned char ID = 5;  
    unsigned char valor = 20;
    int frame = converte_valor(ID, valor);
    while (1) {
        send_sync(&frame, 1);
      
    }
    return NULL;
}

void *TC(void *threadno) {
    unsigned char ID = 6;  
    unsigned char valor = 30;
    int frame = converte_valor(ID, valor);
    while (1) {
        send_sync(&frame, 2);
        
    }
    return NULL;
}

void *TD(void *threadno) {
    unsigned char ID = 7;  
    unsigned char valor = 40;
    int frame = converte_valor(ID, valor);
    while (1) {
        send_sync(&frame, 3);
       
    }
    return NULL;
}

// Threads de destino (recebendo frames)
void *TE(void *threadno) {
    int frame;
    while (1) {
        receive(&frame, 4);
        printf("Thread E recebe valor: %d\n", frame);
    }
    return NULL;
}

void *TF(void *threadno) {
    int frame;
    while (1) {
        receive(&frame, 5);
        printf("Thread F recebe valor: %d\n", frame);
    }
    return NULL;
}

void *TG(void *threadno) {
    int frame;
    while (1) {
        receive(&frame, 6);
        printf("Thread G recebe valor: %d\n", frame);
    }
    return NULL;
}

void *TH(void *threadno) {
    int frame;
    while (1) {
        receive(&frame, 7);
        printf("Thread H recebe valor: %d\n", frame);
    }
    return NULL;
}

// Aguarda até que uma mensagem seja recebida em qualquer canal de origem
int alt_wait(int Q, int canal[]) {
    while (1) {
        for (int i = 0; i < Q; i++) {
            if (canal[i] != -1) {
                return i;
            }
        }
    }
    return -1;
}

// Mailbox: recebe mensagens dos canais e repassa para os destinos corretos
void *T_Mailbox(void *threadno) {
    int buffer[BUFFER];
    int count = 0;
    int chan;

    while (1) {
        // Receber mensagens das threads de origem até encher o buffer
        for (count = 0; count < BUFFER; count++) {
            chan = alt_wait(4, canal); 
            receive(&buffer[count], chan);
        }

        // Enviar mensagens para os destinatários
        for (int i = 0; i < BUFFER; i++) {
            unsigned char dest, valor;
            encontra_bytes(buffer[i], &dest, &valor); // Extrai ID do destino e valor da mensagem
            
            // Verific a se o destino está no intervalo correto
            if (dest >= 4 && dest <= 7) {
                int valor_int = (int)valor; // Converte o byte recebido para inteiro
                send_sync(&valor_int, dest); 
            } else {
                printf("Erro: destino %d inválido!\n", dest);
            }
        }
    }
    return NULL;
}

int main(void) {
    pthread_t thread_TA, thread_TB, thread_TC, thread_TD;
    pthread_t thread_TE, thread_TF, thread_TG, thread_TH;
    pthread_t thread_mailbox;

    // Criando as threads de envio
    pthread_create(&thread_TA, NULL, TA, NULL);
    pthread_create(&thread_TB, NULL, TB, NULL);
    pthread_create(&thread_TC, NULL, TC, NULL);
    pthread_create(&thread_TD, NULL, TD, NULL);

    // Criando as threads de recebimento
    pthread_create(&thread_TE, NULL, TE, NULL);
    pthread_create(&thread_TF, NULL, TF, NULL);
    pthread_create(&thread_TG, NULL, TG, NULL);
    pthread_create(&thread_TH, NULL, TH, NULL);

    // Criando a thread da Mailbox
    pthread_create(&thread_mailbox, NULL, T_Mailbox, NULL);

    // Esperando todas as trheads terminarem
    pthread_join(thread_TA, NULL);
    pthread_join(thread_TB, NULL);
    pthread_join(thread_TC, NULL);
    pthread_join(thread_TD, NULL);
    pthread_join(thread_TE, NULL);
    pthread_join(thread_TF, NULL);
    pthread_join(thread_TG, NULL);
    pthread_join(thread_TH, NULL);
    pthread_join(thread_mailbox, NULL);

    return 0;
}
