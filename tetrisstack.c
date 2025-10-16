#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5 // Capacidade máxima da fila de peças

// Estrutura para representar uma peça do jogo
typedef struct {
    char nome; // 'I', 'O', 'T', 'L', etc.
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// --- Protótipos das Funções ---
void inicializarFila(Fila* f);
void enfileirar(Fila* f);
void desenfileirar(Fila* f);
void exibirFila(const Fila* f);
Peca gerarPeca();

int main() {
    Fila filaDePecas;
    int opcao;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    inicializarFila(&filaDePecas);

    // Preenche a fila com as peças iniciais
    printf("Gerando fila inicial de pecas...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&filaDePecas);
    }

    do {
        exibirFila(&filaDePecas);

        printf("\nOpcoes:\n");
        printf("1. Jogar peca (dequeue)\n");
        printf("2. Inserir nova peca (enqueue)\n");
        printf("0. Sair\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&filaDePecas);
                break;
            case 2:
                enfileirar(&filaDePecas);
                break;
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
    f->quantidade = 0;
}

// Gera uma nova peça com tipo aleatório e ID sequencial
Peca gerarPeca() {
    static int proximoId = 0; // ID único que incrementa a cada chamada
    Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};

    novaPeca.id = proximoId++;
    novaPeca.nome = tiposDePeca[rand() % 7]; // Sorteia um tipo

    return novaPeca;
}

// Adiciona uma peça ao final da fila (se houver espaco)
void enfileirar(Fila* f) {
    if (f->quantidade >= TAMANHO_FILA) {
        printf("\nAVISO: A fila de proximas pecas esta cheia!\n");
        return;
    }

    // Avança o ponteiro 'fim' de forma circular
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = gerarPeca();
    f->quantidade++;
}

// Remove e "joga" a peça do início da fila
void desenfileirar(Fila* f) {
    if (f->quantidade == 0) {
        printf("\nAVISO: A fila esta vazia, nao ha pecas para jogar!\n");
        return;
    }

    Peca pecaJogada = f->pecas[f->inicio];
    printf("\n--> Peca [%c %d] jogada!\n", pecaJogada.nome, pecaJogada.id);

    // Avança o ponteiro 'inicio' de forma circular
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
}

// Mostra o estado atual de todas as peças na fila
void exibirFila(const Fila* f) {
    printf("\n--- Fila de Pecas Futuras ---\n");
    if (f->quantidade == 0) {
        printf("[ Fila Vazia ]\n");
        return;
    }

    // Percorre os elementos da fila na ordem correta
    for (int i = 0; i < f->quantidade; i++) {
        int indice = (f->inicio + i) % TAMANHO_FILA;
        printf("[%c %d] ", f->pecas[indice].nome, f->pecas[indice].id);
    }
    printf("\n-----------------------------\n");
}