#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para representar um cômodo da mansão (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda; // Caminho para a sala à esquerda
    struct Sala *direita;  // Caminho para a sala à direita
} Sala;

// --- Funções de Criação da Estrutura ---

/*
 * Cria e inicializa uma nova sala (nó da árvore).
 * nome O nome da sala.
 * esquerda - O ponteiro para a sala à esquerda (NULL se não houver).
 * direita - O ponteiro para a sala à direita (NULL se não houver).
 * Sala* - O ponteiro para a nova sala criada.
 */
Sala* criarSala(const char *nome, Sala *esquerda, Sala *direita) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala");
        exit(EXIT_FAILURE);
    }
    // Copia o nome para o campo 'nome' da estrutura
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0'; // Garantir terminação de string
    
    novaSala->esquerda = esquerda;
    novaSala->direita = direita;
    
    return novaSala;
}

/*
 * Libera a memória alocada para toda a árvore de salas.
 * raiz O nó raiz da subárvore a ser liberada.
 */
void liberarMansao(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }
    // Percorre e libera as subárvores à esquerda e à direita (pós-ordem)
    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);
    
    // Libera a memória do nó atual
    free(raiz);
}

// --- Funções de Exploração e Jogo ---

/**
 * Permite ao jogador interagir e explorar a mansão.
 * atual O nó (sala) atual em que o jogador se encontra.
 */
void explorarSalas(Sala *atual) {
    char escolha;
    
    while (atual != NULL) {
        // Exibe o nome da sala atual
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se é um nó folha (fim do caminho)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\n--- Fim do Caminho: Este é um beco sem saída na mansão. ---\n");
            printf("Voltando ao Hall de Entrada...\n");
            return; // Sai da exploração atual (retorna ao 'main' ou chamada anterior se fosse recursivo)
        }

        // Exibe as opções de navegação
        printf("Escolha o próximo caminho:\n");
        if (atual->esquerda != NULL) {
            printf("  [e] Ir para a esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("  [d] Ir para a direita\n");
        }
        printf("  [s] Sair da exploração\n");
        printf("Sua escolha: ");
        
        // Leitura da escolha do usuário
        // Usamos ' %c' para consumir um possível caractere de nova linha pendente
        if (scanf(" %c", &escolha) != 1) {
             // Lida com erro de leitura, embora incomum aqui
             while(getchar() != '\n'); // Limpa o buffer de entrada
             continue;
        }

        // Lógica de navegação
        switch (escolha) {
            case 'e':
            case 'E':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda; // Move para a sala da esquerda
                } else {
                    printf("Não há caminho para a esquerda a partir desta sala.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (atual->direita != NULL) {
                    atual = atual->direita; // Move para a sala da direita
                } else {
                    printf("Não há caminho para a direita a partir desta sala.\n");
                }
                break;

            case 's':
            case 'S':
                printf("\nSaiu da exploração. Volte sempre!\n");
                return; // Encerra a função
                
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
        
        // Limpa o buffer de entrada para evitar problemas de leitura na próxima iteração
        while(getchar() != '\n');
    }
}

// --- Função Principal ---

int main() {
    printf("==========================================\n");
    printf("     EXPLORAÇÃO DA MANSÃO (Árvore Binária)\n");
    printf("==========================================\n");

    // 
    // A construção da árvore (montagem estática)

    // Nível 3 (Folhas - Beco sem saída)
    Sala *sala_banheiro_norte = criarSala("Banheiro Norte", NULL, NULL);
    Sala *sala_quarto_vestir = criarSala("Quarto de Vestir", NULL, NULL);
    Sala *sala_adega = criarSala("Adega", NULL, NULL);
    Sala *sala_jardim_inverno = criarSala("Jardim de Inverno", NULL, NULL);

    // Nível 2
    Sala *sala_quarto_mestre = criarSala("Quarto Mestre", sala_banheiro_norte, sala_quarto_vestir);
    Sala *sala_biblioteca = criarSala("Biblioteca", sala_adega, sala_jardim_inverno);

    // Nível 1
    Sala *sala_sala_jantar = criarSala("Sala de Jantar", NULL, sala_quarto_mestre);
    Sala *sala_sala_musica = criarSala("Sala de Música", sala_biblioteca, NULL);
    
    // Nível 0 (Raiz)
    Sala *sala_hall_entrada = criarSala("Hall de Entrada", sala_sala_jantar, sala_sala_musica);

    // Começa o jogo
    explorarSalas(sala_hall_entrada);

    // Limpeza da memória (muito importante!)
    liberarMansao(sala_hall_entrada);
    printf("\nMemória da mansão liberada com sucesso.\n");

    return 0;
}
