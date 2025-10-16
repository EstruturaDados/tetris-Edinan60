#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3

// Estrutura para representar uma peça do jogo
typedef struct {
    char nome; // 'I', 'O', 'T', 'L', etc.
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
} Pilha;

// --- Protótipos das Funções ---
void inicializarFila(Fila* f);
void inicializarPilha(Pilha* p);
Peca gerarPeca();
void enfileirar(Fila* f);
Peca desenfileirar(Fila* f);
void empilhar(Pilha* p, Peca peca);
Peca desempilhar(Pilha* p);
void exibirEstado(const Fila* f, const Pilha* p);

int main() {
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int opcao;

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Preenche a fila com as peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&filaDePecas);
    }

    do {
        exibirEstado(&filaDePecas, &pilhaDeReserva);

        printf("\nOpcoes de Acao:\n");
        printf("1. Jogar peca\n");
        printf("2. Reservar peca\n");
        printf("3. Usar peca reservada\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&filaDePecas);
                if(jogada.id != -1) {
                    printf("\n--> Peca [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enfileirar(&filaDePecas); // Adiciona uma nova peça para manter a fila cheia
                }
                break;
            }
            case 2: {
                if (pilhaDeReserva.topo < CAPACIDADE_PILHA - 1) {
                    Peca reservada = desenfileirar(&filaDePecas);
                    if(reservada.id != -1){
                        empilhar(&pilhaDeReserva, reservada);
                        printf("\n--> Peca [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                        enfileirar(&filaDePecas); // Mantém a fila cheia
                    }
                } else {
                    printf("\nAVISO: A pilha de reserva esta cheia!\n");
                }
                break;
            }
            case 3: {
                Peca usada = desempilhar(&pilhaDeReserva);
                 if(usada.id != -1) {
                    printf("\n--> Peca [%c %d] da reserva foi usada!\n", usada.nome, usada.id);
                }
                break;
            }
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// Configura os valores iniciais da fila
void inicializarFila(Fila* f) {
    f->inicio = 0;
    f->fim = -1;
}

// Configura a pilha como vazia
void inicializarPilha(Pilha* p) {
    p->topo = -1;
}

// Gera uma nova peça com tipo aleatório e ID sequencial
Peca gerarPeca() {
    static int proximoId = 0;
    Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    novaPeca.id = proximoId++;
    novaPeca.nome = tiposDePeca[rand() % 7];
    return novaPeca;
}

// Adiciona uma peça ao final da fila
void enfileirar(Fila* f) {
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->itens[f->fim] = gerarPeca();
}

// Remove e retorna a peça do início da fila
Peca desenfileirar(Fila* f) {
    // Verifica se a fila não está vazia (condição impossível no fluxo do jogo, mas boa prática)
    Peca pecaVazia = {' ', -1};
    if ((f->fim + 1) % TAMANHO_FILA == f->inicio && f->itens[f->inicio].id == 0) {
        printf("\nAVISO: A fila esta vazia!\n");
        return pecaVazia;
    }
    Peca pecaRemovida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    return pecaRemovida;
}

// Adiciona uma peça ao topo da pilha
void empilhar(Pilha* p, Peca peca) {
    p->topo++;
    p->itens[p->topo] = peca;
}

// Remove e retorna a peça do topo da pilha
Peca desempilhar(Pilha* p) {
    Peca pecaVazia = {' ', -1};
    if (p->topo == -1) {
        printf("\nAVISO: A pilha de reserva esta vazia!\n");
        return pecaVazia;
    }
    Peca pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}

// Mostra o estado atual da fila e da pilha
void exibirEstado(const Fila* f, const Pilha* p) {
    printf("\n--- ESTADO ATUAL ---\n");
    // Exibe a Fila
    printf("Fila de pecas: ");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        int indice = (f->inicio + i) % TAMANHO_FILA;
        printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
    }
    printf("\n");

    // Exibe a Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (p->topo == -1) {
        printf("[ Vazia ]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n--------------------\n");
}