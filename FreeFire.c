// Objetivos: Criar um programa em C que simule a interacao com uma mochila de
// capacidade limitada, podendo carregar ate 10 itens diferentes.
//
// Requisitos:
// 1. Criação da struct: definir uma struct chamada Item com os campos char nome[30], char tipo[20] e int quantidade.
// 2. Cadastro de itens: o sistema deve permitir que o jogador cadastre até 10 itens informando nome, tipo (ex: arma, munição e cura) e quantidade.
// 3. Remoção de itens: permitir que o jogador exclua um item da mochila, informando seu nome.
// 4. Listagem dos itens registrados com seus dados: o que deve ocorrer após cada operação.
// 5. Busca sequencial: implementar uma função de busca sequencial que localize um item na mochila com base no nome e exiba seus dados.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CAPACIDADE_MOCHILA 10
#define TAM_NOME 30
#define TAM_TIPO 20


// Estrutura que representa cada item guardado na mochila.
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;

} Item;

// Remove caracteres que ficaram no buffer depois de usar scanf.
void limparBufferEntrada() {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

// Le uma string com fgets e remove o '\n' que pode ficar no final.
void lerTexto(const char *mensagem, char texto[], int tamanho) {
    printf("%s", mensagem);
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

// Procura um item pelo nome e retorna sua posicao no vetor.
// Se nao encontrar, retorna -1.
int buscarItem(Item mochila[], int contador, const char nome[]) {
    for (int i = 0; i < contador; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }

    return -1;
}

void inserirItem(Item mochila[], int *contador) {
    Item novoItem;
    int posicao;

    // Coleta os dados do novo item antes de inserir ou atualizar a mochila.
    lerTexto("Nome do item: ", novoItem.nome, TAM_NOME);
    lerTexto("Tipo do item (arma, municao, cura, etc.): ", novoItem.tipo, TAM_TIPO);

    printf("Quantidade disponivel: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada();

    // Impede que itens sejam cadastrados com quantidade invalida.
    if (novoItem.quantidade <= 0) {
        printf("Erro: A quantidade deve ser maior que zero.\n");
        return;
    }

    posicao = buscarItem(mochila, *contador, novoItem.nome);

    // Se o item ja existe, soma a quantidade em vez de ocupar outra posicao.
    if (posicao != -1) {
        mochila[posicao].quantidade += novoItem.quantidade;
        printf("Item ja existia. Quantidade atualizada para %d.\n", mochila[posicao].quantidade);
        return;
    }

    // So permite novos tipos de item enquanto houver espaco no vetor.
    if (*contador >= CAPACIDADE_MOCHILA) {
        printf("Erro: A mochila esta cheia. Nao e possivel inserir outro item diferente.\n");
        return;
    }

    // Insere o item na proxima posicao livre e aumenta o contador.
    mochila[*contador] = novoItem;
    (*contador)++;

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

void removerItem(Item mochila[], int *contador) {
    char nome[TAM_NOME];
    int posicao;
    int quantidadeRemover;

    // Antes de procurar qualquer item, verifica se ha algo para remover.
    if (*contador == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    lerTexto("Digite o nome do item a remover: ", nome, TAM_NOME);

    posicao = buscarItem(mochila, *contador, nome);

    if (posicao == -1) {
        printf("Erro: Item nao encontrado na mochila.\n");
        return;
    }

    printf("Quantidade a remover: ");
    scanf("%d", &quantidadeRemover);
    limparBufferEntrada();

    if (quantidadeRemover <= 0) {
        printf("Erro: A quantidade deve ser maior que zero.\n");
        return;
    }

    // Se remover menos do que existe, apenas diminui a quantidade.
    if (quantidadeRemover < mochila[posicao].quantidade) {
        mochila[posicao].quantidade -= quantidadeRemover;
        printf("Quantidade atualizada para %d.\n", mochila[posicao].quantidade);
        return;
    }

    // Se remover tudo ou mais, apaga o item deslocando os proximos para a esquerda.
    for (int i = posicao; i < *contador - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*contador)--;
    printf("Item removido da mochila.\n");
}

void listarItens(const Item mochila[], int contador) {
    // Nao imprime a tabela se nao houver itens cadastrados.
    if (contador == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("\n       --- ITENS NA MOCHILA (%d/10) ---\n", contador);
    printf("----------------------------------------------------------\n");
    printf("%-20s | %-20s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------------\n");

    // Percorre apenas as posicoes usadas da mochila.
    for (int i = 0; i < contador; i++) {
        printf("%-20s | %-20s | %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    printf("----------------------------------------------------------\n");
    
    printf("\nPressione Enter para continuar...");
    getchar();
}

int main() {
    // Vetor que funciona como lista linear da mochila.
    Item mochila[CAPACIDADE_MOCHILA];

    // contador indica quantas posicoes do vetor estao ocupadas.
    int contador = 0;
    int opcao;

    // Repete o menu ate que o usuario escolha a opcao de sair.
    do {
        printf("\n======================================================");
        printf("\n     MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("======================================================\n");
        printf("\nItens na Mochila: %d/10\n", contador);
        printf("\n\n1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item por Nome\n");
        printf("0. Sair do Programa\n");
        printf("\n--------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        // Chama a funcao correspondente a opcao escolhida.
        switch (opcao) {
            case 1:
                // Opcao 1: adicionar um item novo ou somar quantidade se o item ja existir.
                printf("\n--- Adicionar Novo Item ---\n");
                inserirItem(mochila, &contador);
                listarItens(mochila, contador);
                break;
            case 2:
                // Opcao 2: remover quantidade de um item ou apagar o item se a quantidade chegar a zero.
                printf("\n--- Remover Item ---\n");
                removerItem(mochila, &contador);
                listarItens(mochila, contador);
                break;
            case 3:
                // Opcao 3: exibir a lista atual de itens na mochila.
                printf("\n--- Listar Itens ---\n");
                listarItens(mochila, contador);
                break;
            case 4:
                // Opcao 4: buscar um item pelo nome e mostrar seus dados.
                printf("\n--- Buscar Item na Mochila ---\n");
                {
                    char nomeBusca[TAM_NOME];
                    lerTexto("Digite o nome do item que deseja buscar: ", nomeBusca, TAM_NOME);
                    int posicao = buscarItem(mochila, contador, nomeBusca);
                    if (posicao != -1) {
                        printf("\n--- Item Encontrado! ---\n");
                        printf("Nome: %s\n", mochila[posicao].nome);
                        printf("Tipo: %s\n", mochila[posicao].tipo);
                        printf("Quantidade: %d\n", mochila[posicao].quantidade);
                        printf("--------------------------\n");
                    } else {
                        printf("Item nao encontrado na mochila.\n");
                    }
                    printf("\nPressione Enter para continuar...");
                    getchar();
                }
                break;
            case 0:
                // Opcao 0: sair do programa.
                printf("Saindo...\n");
                break;
            default:
                // Qualquer outro valor invalido retorna ao menu.
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
