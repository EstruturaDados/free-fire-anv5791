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
    int prioridade;
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



// Busca binaria por nome. Requer que a mochila esteja ordenada por nome.
int buscaBinariaNome(const Item mochila[], int contador, const char nome[]) {
    int inicio = 0;
    int fim = contador - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(mochila[meio].nome, nome);
        if (comparacao == 0) {
            return meio;
        } else if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}

void inserirItem(Item mochila[], int *contador, int *estadoOrdenacao) {
    Item novoItem;
    int posicao;

    // Coleta os dados do novo item antes de inserir ou atualizar a mochila.
    lerTexto("Nome do item: ", novoItem.nome, TAM_NOME);
    lerTexto("Tipo do item (Estrutural, Eletronico, Energia): ", novoItem.tipo, TAM_TIPO);

    printf("Quantidade disponivel: ");
    scanf("%d", &novoItem.quantidade);
    printf("Prioridade de Montagem (1-5): ");
    scanf("%d", &novoItem.prioridade);
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
    *estadoOrdenacao = 0; // A mochila perde a ordenação garantida

    printf("\nComponente '%s' adicionado com sucesso!\n", novoItem.nome);
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

    printf("\n             --- INVENTARIO ATUAL (%d/10) ---\n", contador);
    printf("-----------------------------------------------------------------------\n");
    printf("%-20s | %-20s | %10s | %10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("-----------------------------------------------------------------------\n");

    // Percorre apenas as posicoes usadas da mochila.
    for (int i = 0; i < contador; i++) {
        printf("%-20s | %-20s | %10d | %10d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("-----------------------------------------------------------------------\n");


    printf("\nPressione Enter para continuar...");
    getchar();
}

void ordenarItens_nome(Item mochila[], int contador, int *estadoOrdenacao) {
    // Implementar ordenação por nome       
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                Item temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    if (contador > 0) {
        *estadoOrdenacao = 1;
    }
}

void ordenarItens_tipo(Item mochila[], int contador, int *estadoOrdenacao) {
    // Implementar ordenação por tipo       
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            if (strcmp(mochila[j].tipo, mochila[j + 1].tipo) > 0) {
                Item temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    if (contador > 0) {
        *estadoOrdenacao = 0; // Mantém como NÃO ORDENADO para a lógica de busca
    }
}

void ordenarPrioridade(Item mochila[], int contador, int *estadoOrdenacao) {
    // Implementar ordenação por prioridade
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            if (mochila[j].prioridade < mochila[j + 1].prioridade) {
                Item temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    if (contador > 0) {
        *estadoOrdenacao = 0; // Mantém como NÃO ORDENADO para a lógica de busca
    }
}

int main() {
    // Vetor que funciona como lista linear da mochila.
    Item mochila[CAPACIDADE_MOCHILA];

    // contador indica quantas posicoes do vetor estao ocupadas.
    int contador = 0;
    int opcao;
    int opcaoOrganizacao;
    
    // 0 = Nao Ordenado, 1 = Nome, 2 = Tipo, 3 = Prioridade
    int estadoOrdenacao = 0;

    // Repete o menu ate que o usuario escolha a opcao de sair.
    do {
        printf("\n======================================================");
        printf("\n     PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
        printf("======================================================\n");
        printf("\nItens na Mochila: %d/10\n", contador);
        
        printf("\nStatus da Ordenacao por nome: %s\n", (estadoOrdenacao == 1) ? "ORDENADO" : "NÃO ORDENADO");
        
        printf("\n\n1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Buscar Binaria por Componente-Chave (por Nome)\n");
        printf("0.  ATIVAR TORRE DE FUGA (Sair do Programa)\n");
        printf("\n--------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        // Chama a funcao correspondente a opcao escolhida.
        switch (opcao) {
            case 1:
                // Opcao 1: adicionar um item novo ou somar quantidade se o item ja existir.
                printf("\n--- Coletando Novo Componente ---\n");
                inserirItem(mochila, &contador, &estadoOrdenacao);
                listarItens(mochila, contador);
                break;
            case 2:
                // Opcao 2: remover quantidade de um item ou apagar o item se a quantidade chegar a zero.
                printf("\n--- Descartando Componente ---\n");
                removerItem(mochila, &contador);
                listarItens(mochila, contador);
                break;
            case 3:
                // Opcao 3: exibir a lista atual de itens na mochila.
                printf("\n--- Listar Componentes ---\n");
                listarItens(mochila, contador);
                break;
            case 4:
                // Opcao 4: organizar a mochila (ordenar os itens).
                do {
                    printf("\n--- Estrategia de Organização ---");
                    printf("\nComo deseja ordenar os componentes?");
                    printf("\n1. Por Nome (Ordem Alfabética)");
                    printf("\n2. Por Tipo");
                    printf("\n3. Por Prioridade de Montagem");
                    printf("\n0. Cancelar");
                    printf("\nEscolha o criterio: ");
                    scanf("%d", &opcaoOrganizacao);
                    limparBufferEntrada();  

                    switch (opcaoOrganizacao) {
                        case 1:
                            ordenarItens_nome(mochila, contador, &estadoOrdenacao);
                            listarItens(mochila, contador);
                            break;
                        case 2:
                            ordenarItens_tipo(mochila, contador, &estadoOrdenacao);
                            listarItens(mochila, contador);
                            break;
                        case 3:
                            ordenarPrioridade(mochila, contador, &estadoOrdenacao);
                            listarItens(mochila, contador);
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao invalida. Tente novamente.\n");
                            break;
                    }
                } while (opcaoOrganizacao < 0 || opcaoOrganizacao > 3);
                break;
            case 5:
                // Opcao 5: buscar um item pelo nome e mostrar seus dados.
                if (estadoOrdenacao != 1) {
                        printf("\nALERTA: A busca binaria requer que a mochila esteja ordenada por NOME.\n");
                        printf("Por favor, use a opcao 4 (opcao 1 - Por Nome) para organizar a mochila primeiro.\n");
                        break;
                    }
                printf("\n--- Busca Binaria por Componentes-Chave ---\n");
                {                       
                    char nomeBusca[TAM_NOME];
                    lerTexto("Nome do Componente a buscar: ", nomeBusca, TAM_NOME);
                    int posicao = buscaBinariaNome(mochila, contador, nomeBusca);
                    if (posicao != -1) {
                        printf("\n--- Componente-Chave Encontrado! ---\n");
                        printf("Nome: %s\n", mochila[posicao].nome);
                        printf("Tipo: %s\n", mochila[posicao].tipo);
                        printf("Quantidade: %d\n", mochila[posicao].quantidade);
                        printf("Prioridade: %d\n", mochila[posicao].prioridade);
                        printf("--------------------------\n");
                    } else {
                        printf("Componente-Chave nao encontrado na mochila.\n");
                    }
                    printf("\nPressione Enter para continue...");
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
