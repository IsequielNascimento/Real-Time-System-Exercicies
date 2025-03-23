# Repositório de Sistemas de Tempo Real

Este repositório contém implementações de laboratórios da disciplina de Sistemas de Tempo Real, abordando conceitos de concorrência, comunicação entre threads e tolerância a falhas.

## Estrutura do Repositório

- **LAB 1**: Implementações de concorrência utilizando tarefas em Ada e threads em C.
  - `main.adb`: Implementa uma sincronização entre tarefas em Ada utilizando flags de controle.
  - `main.c`: Implementação equivalente em C utilizando pthreads.

- **LAB 2**: Implementação de comunicação entre processos através de um canal compartilhado.
  - `main.c`: Utiliza um canal para enviar e receber mensagens de forma assíncrona, implementando um mecanismo de votação e tolerância a falhas.

- **LAB 3**: Implementação de uma Mailbox para gerenciar a comunicação entre threads.
  - `main.c`: Utiliza threads para simular um sistema de comunicação assíncrona entre processos, garantindo a entrega das mensagens através de uma Mailbox intermediária.

## Como Executar os Códigos

Os códigos foram executados nas plataformas onlines: onlinegdb.com para C e onecompiler.com/ada para ada

<!-- 
### Compilando e Executando o LAB 1

#### Ada
1. Compilar:
   ```sh
   gnatmake test_task.adb
   ```
2. Executar:
   ```sh
   ./test_task
   ```

#### C
1. Compilar:
   ```sh
   gcc -o lab1 threads_lab1.c -lpthread
   ```
2. Executar:
   ```sh
   ./lab1
   ```

### Compilando e Executando o LAB 2
1. Compilar:
   ```sh
   gcc -o lab2 threads_lab2.c -lpthread
   ```
2. Executar:
   ```sh
   ./lab2
   ```

### Compilando e Executando o LAB 3
1. Compilar:
   ```sh
   gcc -o lab3 threads_lab3.c -lpthread
   ```
2. Executar:
   ```sh
   ./lab3
   ``` -->
<!-- 
## Requisitos

- **Para Ada**: GNAT (compilador Ada)
- **Para C**: GCC e a biblioteca `pthread` -->

## Autor
Repositório criado para a disciplina de Sistemas de Tempo Real.

