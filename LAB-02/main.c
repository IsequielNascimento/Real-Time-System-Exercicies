#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_VERSOES 3

// Estruturas para comunicação entre threads
int canal[NUM_VERSOES] = {-1, -1, -1};
int status[NUM_VERSOES] = {0, 0, 0};
pthread_mutex_t mutex_canal = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_canal = PTHREAD_COND_INITIALIZER;

// Funções de envio e recepção
void send_async(int *buf, int valor) {
    pthread_mutex_lock(&mutex_canal);
    *buf = valor;
    pthread_cond_signal(&cond_canal);
    pthread_mutex_unlock(&mutex_canal);
}

void receive(int *buf, int *valor) {
    pthread_mutex_lock(&mutex_canal);
    while (*buf == -1) {
        pthread_cond_wait(&cond_canal, &mutex_canal);
    }
    *valor = *buf;
    *buf = -1;
    pthread_mutex_unlock(&mutex_canal);
}

// Funções das versões
void *versao_1(void *arg) {
    int voto = 5 + 5;
    send_async(&canal[0], voto);

    int status_recebido;
    receive(&status[0], &status_recebido);

    if (status_recebido == 1) {
        printf("Versão 1: Voto incorreto, finalizando.\n");
    } else {
        printf("Versão 1: Voto correto, continuando execução.\n");
    }
    return NULL;
}

void *versao_2(void *arg) {
    int voto = 2 * 5;
    send_async(&canal[1], voto);

    int status_recebido;
    receive(&status[1], &status_recebido);

    if (status_recebido == 1) {
        printf("Versão 2: Voto incorreto, finalizando.\n");
    } else {
        printf("Versão 2: Voto correto, continuando execução.\n");
    }
    return NULL;
}

void *versao_3(void *arg) {
    int voto = 3 + 7;
    send_async(&canal[2], voto);

    int status_recebido;
    receive(&status[2], &status_recebido);

    if (status_recebido == 1) {
        printf("Versão 3: Voto incorreto, finalizando.\n");
    } else {
        printf("Versão 3: Voto correto, continuando execução.\n");
    }
    return NULL;
}

// Função de comparação
int compara(int vetor_comp[], int *versao_erro) {
    int votos[NUM_VERSOES] = {0};
    for (int i = 0; i < NUM_VERSOES; i++) {
        votos[vetor_comp[i]]++;
    }

    int voto_majoritario = -1;
    for (int i = 0; i < NUM_VERSOES; i++) {
        if (votos[vetor_comp[i]] > 1) {
            voto_majoritario = vetor_comp[i];
            break;
        }
    }

    for (int i = 0; i < NUM_VERSOES; i++) {
        if (vetor_comp[i] != voto_majoritario) {
            *versao_erro = i;
        }
    }

    return voto_majoritario;
}

// Função do driver
void *driver(void *arg) {
    int vetor_comp[NUM_VERSOES];

    for (int i = 0; i < NUM_VERSOES; i++) {
        receive(&canal[i], &vetor_comp[i]);
    }

    int versao_erro;
    int voto_majoritario = compara(vetor_comp, &versao_erro);

    for (int i = 0; i < NUM_VERSOES; i++) {
        if (i == versao_erro) {
            send_async(&status[i], 1);
        } else {
            send_async(&status[i], 0);
        }
    }

    printf("Voto majoritário: %d\n", voto_majoritario);
    if (versao_erro != -1) {
        printf("Versão com voto minoritário: Versão %d\n", versao_erro + 1);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_VERSOES + 1];

    pthread_create(&threads[0], NULL, versao_1, NULL);
    pthread_create(&threads[1], NULL, versao_2, NULL);
    pthread_create(&threads[2], NULL, versao_3, NULL);
    pthread_create(&threads[3], NULL, driver, NULL);

    for (int i = 0; i < NUM_VERSOES + 1; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
