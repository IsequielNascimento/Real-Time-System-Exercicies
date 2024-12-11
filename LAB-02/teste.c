#include<stdio.h>
#include<stddef.h>
#include<unistd.h>
#include<pthread.h>

int canal[6] = {-1, -1, -1, -1, -1, -1};

void send_async(int *buf, int c)
{
	canal[c] =* buf;
	return;
}

void receive(int *buf, int c)
{
	while(canal[c] == -1);
	*buf = canal[c];
	canal[c] = -1;
	return;
}

//FUNÇÃO DE COMPARAÇÃO DE VETORES

int comparacao(int vetor[], int *versao_com_erro)
{
	if((vetor[0]==vetor[1]) && (vetor[1]==vetor[2]))
	{
		*versao_com_erro = 0;
		return vetor[0];
	}
	
	if((vetor[0]==vetor[1]) && (vetor[1]!=vetor[2]))
	{
		*versao_com_erro = 3;
		return vetor[0];
	}
	
	if((vetor[0]!=vetor[1]) && (vetor[1]==vetor[2]))
	{
		*versao_com_erro = 1;
		return vetor[1];
	}
	
	if((vetor[0]==vetor[2]) && (vetor[1]!=vetor[2]))
	{
		*versao_com_erro = 2;
		return vetor[0];
	}
}

// VERSÃO A

void *thread_code_a(void *threadno)
{
	int voto = 10; 
	int status;
	send_async(&voto, 0);
	receive(&status, 3);
	if(status == 0){
		printf("\nTA tem continuidade - Prossegue!");
		while(1);
	}
	else{
		printf("\nTA gera erro!\n");
		return NULL;
	}
}

// VERSÃO B

void *thread_code_b(void *threadno)
{
	int voto = 8; 
	int status;
	send_async(&voto, 1);
	receive(&status, 4);
	if(status == 0){
		printf("\nTB tem continuidade - Prossegue!");
		while(1);
	}
	else{
		printf("\nTB gera erro!\n");
		return NULL;
	}
}

// VERSÃO C

void *thread_code_c(void *threadno)
{
	int voto = 10; 
	int status;
	send_async(&voto, 2);
	receive(&status, 5);
	if(status == 0){
		printf("\nTC tem continuidade - Prossegue!");
		while(1);
	}
	else{
		printf("\nTC gera erro!\n");
		return NULL;
	}
}

// PROCESSO DRIVER

void *driver(void *threadno)
{
	int vetor_comp[3];
	int statusok = 0;
	int statuserro = 1;
	int voto_majoritario; 
	int versao_erro;
	
	receive(&vetor_comp[0], 0);
	receive(&vetor_comp[1], 1);
	receive(&vetor_comp[2], 2);
	
	voto_majoritario = comparacao(vetor_comp, &versao_erro);
	printf("\nResposta Correta: %d\n\n", voto_majoritario);
	
	if(versao_erro == 0)
	{
		send_async(&statusok, 3);
		send_async(&statusok, 4);
		send_async(&statusok, 5);
	}
	
	if(versao_erro == 1)
	{
		send_async(&statuserro, 3);
		send_async(&statusok, 4);
		send_async(&statusok, 5);
	}
	
	if(versao_erro == 2)
	{
		send_async(&statusok, 3);
		send_async(&statuserro, 4);
		send_async(&statusok, 5);
	}
	
	if(versao_erro == 3)
	{
		send_async(&statusok, 3);
		send_async(&statusok, 4);
		send_async(&statuserro, 5);
	}
	return NULL;
}

int main(void)
{
	pthread_t TA;
	pthread_t TB;
	pthread_t TC;
	pthread_t TD;
	
	pthread_create(&TA, NULL, thread_code_a, NULL);
	pthread_create(&TB, NULL, thread_code_b, NULL);
	pthread_create(&TC, NULL, thread_code_c, NULL);
	pthread_create(&TD, NULL, driver, NULL);
	
	system("pause");
	return 0;
}