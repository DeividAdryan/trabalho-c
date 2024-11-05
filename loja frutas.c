#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar informações de produtos no mercado de frutas
typedef struct {
    int id;
    char descricao[50];
    int estoque;
    float valor;
} Produto;

// Estrutura de nós para a lista encadeada de produtos
typedef struct NoProduto {
    Produto item;
    struct NoProduto *proximo;
} NoProduto;

// Função para inserir um novo produto na lista encadeada
NoProduto* inserirProduto(NoProduto *inicio) {
    Produto novoItem;
    NoProduto *atual = inicio;

    printf("ID do produto: ");
    scanf("%d", &novoItem.id);

    // Verificar se o ID já existe
    while (atual != NULL) {
        if (atual->item.id == novoItem.id) {
            printf("ID já cadastrado.\n");
            return inicio;
        }
        atual = atual->proximo;
    }

    printf("Nome do produto: ");
    scanf("%s", novoItem.descricao);
    printf("Quantidade em estoque: ");
    scanf("%d", &novoItem.estoque);
    printf("Preço: ");
    scanf("%f", &novoItem.valor);

    // Inserir novo item na lista
    NoProduto *novoNo = (NoProduto*) malloc(sizeof(NoProduto));
    novoNo->item = novoItem;
    novoNo->proximo = inicio;
    return novoNo;
}

// Função para exibir todos os produtos cadastrados
void mostrarProdutos(NoProduto *inicio) {
    NoProduto *atual = inicio;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s, Estoque: %d, Preço: %.2f\n",
               atual->item.id, atual->item.descricao, atual->item.estoque, atual->item.valor);
        atual = atual->proximo;
    }
}

// Função para registrar a venda de um produto e atualizar o estoque
void realizarVenda(NoProduto *inicio) {
    int id, quantidade;
    FILE *arquivo = fopen("registro_vendas.txt", "a");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    printf("ID do produto: ");
    scanf("%d", &id);
    printf("Quantidade a vender: ");
    scanf("%d", &quantidade);

    NoProduto *atual = inicio;
    while (atual != NULL) {
        if (atual->item.id == id) {
            if (atual->item.estoque >= quantidade) {
                atual->item.estoque -= quantidade;
                fprintf(arquivo, "ID: %d, Nome: %s, Quantidade vendida: %d, Valor total: %.2f\n",
                        atual->item.id, atual->item.descricao, quantidade, quantidade * atual->item.valor);
                printf("Venda realizada.\n");
            } else {
                printf("Estoque insuficiente.\n");
            }
            fclose(arquivo);
            return;
        }
        atual = atual->proximo;
    }
    printf("Produto não encontrado.\n");
    fclose(arquivo);
}

// Função para modificar informações de um produto
void modificarProduto(NoProduto *inicio) {
    int id;
    printf("Digite o ID do produto que deseja modificar: ");
    scanf("%d", &id);

    NoProduto *atual = inicio;
    while (atual != NULL) {
        if (atual->item.id == id) {
            printf("Nova quantidade em estoque: ");
            scanf("%d", &atual->item.estoque);
            printf("Novo preço: ");
            scanf("%f", &atual->item.valor);
            printf("Dados do produto atualizados com sucesso.\n");
            return;
        }
        atual = atual->proximo;
    }
    printf("Produto com ID %d não encontrado.\n", id);
}

// Função para remover um produto da lista
NoProduto* removerProduto(NoProduto *inicio) {
    int id;
    printf("Digite o ID do produto que deseja remover: ");
    scanf("%d", &id);

    NoProduto *atual = inicio;
    NoProduto *anterior = NULL;

    while (atual != NULL) {
        if (atual->item.id == id) {
            if (atual->item.estoque > 0) {
                printf("Não é possível remover. O produto ainda possui estoque.\n");
                return inicio;
            }
            if (anterior == NULL) {
                inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Produto removido com sucesso.\n");
            return inicio;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Produto com ID %d não encontrado.\n", id);
    return inicio;
}

// Função para salvar os produtos em um arquivo
void salvarProdutos(NoProduto *inicio) {
    FILE *arquivo = fopen("produtos_salvos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    NoProduto *atual = inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d %s %d %.2f\n", atual->item.id, atual->item.descricao, atual->item.estoque, atual->item.valor);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Dados dos produtos salvos com sucesso.\n");
}

// Função para carregar produtos a partir de um arquivo
NoProduto* carregarProdutos() {
    FILE *arquivo = fopen("produtos_salvos.txt", "r");
    if (arquivo == NULL) {
        printf("Arquivo de produtos não encontrado ou vazio.\n");
        return NULL;
    }

    NoProduto *inicio = NULL;
    Produto item;
    while (fscanf(arquivo, "%d %s %d %f", &item.id, item.descricao, &item.estoque, &item.valor) != EOF) {
        NoProduto *novoNo = (NoProduto*) malloc(sizeof(NoProduto));
        novoNo->item = item;
        novoNo->proximo = inicio;
        inicio = novoNo;
    }

    fclose(arquivo);
    printf("Dados dos produtos carregados com sucesso.\n");
    return inicio;
}

// Função principal para o menu de controle
int main() {
    int opcao;
    NoProduto *inicio = carregarProdutos();

    do {
        printf("\nMenu de Opções:\n");
        printf("1. Inserir Produto\n");
        printf("2. Registrar Venda\n");
        printf("3. Exibir Produtos\n");
        printf("4. Modificar Produto\n");
        printf("5. Remover Produto\n");
        printf("6. Salvar Produtos\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inicio = inserirProduto(inicio);
                break;
            case 2:
                realizarVenda(inicio);
                break;
            case 3:
                mostrarProdutos(inicio);
                break;
            case 4:
                modificarProduto(inicio);
                break;
            case 5:
                inicio = removerProduto(inicio);
                break;
            case 6:
                salvarProdutos(inicio);
                break;
            case 7:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida. Por favor, tente novamente.\n");
        }
    } while (opcao != 7);

    return 0;
}
