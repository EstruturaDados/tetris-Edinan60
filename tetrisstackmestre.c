#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3
#define ITENS_TROCA 3

// Estrutura para representar uma peça do jogo
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
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
void trocarPecaAtual(Fila* f, Pilha* p);
void trocaMultipla(Fila* f, Pilha* p);
void exibirEstado(const Fila* f, const Pilha* p);

int main() {
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int opcao;

    srand(time(NULL));

    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Preenche a fila com as peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&filaDePecas);
    }

    do {
        exibirEstado(&filaDePecas, &pilhaDeReserva);

        printf("\nOpcoes Disponiveis:\n");
        printf("1. Jogar peca da fila\n");
        printf("2. Enviar peca da fila para a reserva\n");
        printf("3. Usar peca da reserva\n");
        printf("4. Trocar peca da frente da fila com o topo da pilha\n");
        printf("5. Trocar os %d primeiros da fila com a pilha\n", ITENS_TROCA);
        printf("0. Sair\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&filaDePecas);
                if (jogada.id != -1) {
                    printf("\n--> Peca [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enfileirar(&filaDePecas);
                }
                break;
            }
            case 2: {
                if (pilhaDeReserva.topo < CAPACIDADE_PILHA - 1) {
                    Peca reservada = desenfileirar(&filaDePecas);
                    if (reservada.id != -1) {
                        empilhar(&pilhaDeReserva, reservada);
                        printf("\n--> Peca [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                        enfileirar(&filaDePecas);
                    }
                } else {
                    printf("\nAVISO: A pilha de reserva esta cheia!\n");
                }
                break;
            }
            case 3: {
                Peca usada = desempilhar(&pilhaDeReserva);
                if (usada.id != -1) {
                    printf("\n--> Peca [%c %d] da reserva foi usada!\n", usada.nome, usada.id);
                }
                break;
            }
            case 4:
                trocarPecaAtual(&filaDePecas, &pilhaDeReserva);
                break;
            case 5:
                trocaMultipla(&filaDePecas, &pilhaDeReserva);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Configura os valores iniciais da fila
void inicializarFila(Fila* f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
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

// Adiciona uma peça ao final da fila (se houver espaco)
void enfileirar(Fila* f) {
    if (f->quantidade >= TAMANHO_FILA) return;
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->itens[f->fim] = gerarPeca();
    f->quantidade++;
}

// Remove e retorna a peça do início da fila
Peca desenfileirar(Fila* f) {
    Peca pecaVazia = {' ', -1};
    if (f->quantidade == 0) {
        printf("\nAVISO: A fila esta vazia!\n");
        return pecaVazia;
    }
    Peca pecaRemovida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
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

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(Fila* f, Pilha* p) {
    if (f->quantidade == 0 || p->topo == -1) {
        printf("\nAVISO: Fila e pilha precisam ter pecas para a troca.\n");
        return;
    }
    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("\n--> Troca pontual realizada.\n");
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocaMultipla(Fila* f, Pilha* p) {
    if (f->quantidade < ITENS_TROCA || p->topo < ITENS_TROCA - 1) {
        printf("\nAVISO: Fila e pilha devem ter pelo menos %d pecas.\n", ITENS_TROCA);
        return;
    }

    Peca temp;
    for (int i = 0; i < ITENS_TROCA; i++) {
        int indiceFila = (f->inicio + i) % TAMANHO_FILA;
        int indicePilha = p->topo - i;

        // A troca é feita entre a frente da fila e o topo da pilha
        temp = f->itens[indiceFila];
        f->itens[indiceFila] = p->itens[indicePilha];
        p->itens[indicePilha] = temp;
    }
    printf("\n--> Troca multipla realizada.\n");
}

// Mostra o estado atual da fila e da pilha
void exibirEstado(const Fila* f, const Pilha* p) {
    printf("\n--- ESTADO ATUAL ---\n");
    printf("Fila de pecas: ");
    if (f->quantidade == 0) {
        printf("[ Vazia ]");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int indice = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
        }
    }
    printf("\n");

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