#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char codigo[20];
    char nome[50];
    int quantidade;
    float preco;
    char estado[20];
} Produto;

#define MAX_PRODUTOS 100

void ignorar_primeira_linha(FILE *file) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        perror("Erro ao ler a primeira linha");
    }
}

void copiar_arquivo_ignorando_primeira_linha(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(in);
        return;
    }


    char linha[256];
    while (fgets(linha, sizeof(linha), in) != NULL) {
        fputs(linha, out);
    }

    fclose(in);
    fclose(out);
}

void pesquisar_produto(const char *input_file, const char *codigo) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    ignorar_primeira_linha(in);

    Produto produto;
    int encontrado = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        strncpy(produto.codigo, buffer, sizeof(produto.codigo));
        produto.codigo[strcspn(produto.codigo, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.nome, buffer, sizeof(produto.nome));
        produto.nome[strcspn(produto.nome, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.quantidade = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.preco = atof(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.estado, buffer, sizeof(produto.estado));
        produto.estado[strcspn(produto.estado, "\n")] = '\0'; 

        if (strcmp(produto.codigo, codigo) == 0) {
            printf("%s\n", produto.codigo);
            printf("%s\n", produto.nome);
            printf("%d\n", produto.quantidade);
            printf("%.2f\n", produto.preco);
            printf("%s\n", produto.estado);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Produto com código %s não encontrado.\n", codigo);
    }

    fclose(in);
}

void listar_produto_menor_quantidade(const char *input_file) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    ignorar_primeira_linha(in);

    Produto produto, menor_produto;
    int encontrado = 0;
    int menor_quantidade = __INT_MAX__;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        strncpy(produto.codigo, buffer, sizeof(produto.codigo));
        produto.codigo[strcspn(produto.codigo, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.nome, buffer, sizeof(produto.nome));
        produto.nome[strcspn(produto.nome, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.quantidade = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.preco = atof(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.estado, buffer, sizeof(produto.estado));
        produto.estado[strcspn(produto.estado, "\n")] = '\0'; 

        if (produto.quantidade < menor_quantidade) {
            menor_quantidade = produto.quantidade;
            menor_produto = produto;
            encontrado = 1;
        }
    }

    if (encontrado) {
        printf("%s\n", menor_produto.codigo);
        printf("%s\n", menor_produto.nome);
        printf("%d\n", menor_produto.quantidade);
        printf("%.2f\n", menor_produto.preco);
        printf("%s\n", menor_produto.estado);
    } else {
        printf("Nenhum produto encontrado.\n");
    }

    fclose(in);
}

int comparar_produtos(const void *a, const void *b) {
    const Produto *produto_a = (const Produto *)a;
    const Produto *produto_b = (const Produto *)b;
    return strcmp(produto_a->nome, produto_b->nome);
}

void listar_produtos_por_estado(const char *input_file, const char *estado_procurado) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    ignorar_primeira_linha(in);

    Produto produtos[MAX_PRODUTOS];
    int num_produtos = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in) != NULL && num_produtos < MAX_PRODUTOS) {
        strncpy(produtos[num_produtos].codigo, buffer, sizeof(produtos[num_produtos].codigo));
        produtos[num_produtos].codigo[strcspn(produtos[num_produtos].codigo, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produtos[num_produtos].nome, buffer, sizeof(produtos[num_produtos].nome));
        produtos[num_produtos].nome[strcspn(produtos[num_produtos].nome, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produtos[num_produtos].quantidade = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produtos[num_produtos].preco = atof(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produtos[num_produtos].estado, buffer, sizeof(produtos[num_produtos].estado));
        produtos[num_produtos].estado[strcspn(produtos[num_produtos].estado, "\n")] = '\0'; 

        if (strcmp(produtos[num_produtos].estado, estado_procurado) == 0) {
            num_produtos++;
        }
    }

    fclose(in);

    // Ordena os produtos por nome
    qsort(produtos, num_produtos, sizeof(Produto), comparar_produtos);

    // Imprime os produtos por estado
    for (int i = 0; i < num_produtos; i++) {
        printf("%s\n", produtos[i].codigo);
        printf("%s\n", produtos[i].nome);
        printf("%d\n", produtos[i].quantidade);
        printf("%.2f\n", produtos[i].preco);
        printf("%s\n", produtos[i].estado);
    }

    if (num_produtos == 0) {
        printf("Nenhum produto encontrado para o estado %s.\n", estado_procurado);
    }
}

void encontrar_produto_menor_quantidade_estado(const char *input_file, const char *estado_procurado) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    ignorar_primeira_linha(in);

    Produto produto, menor_produto;
    int encontrado = 0;
    int menor_quantidade = __INT_MAX__;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        strncpy(produto.codigo, buffer, sizeof(produto.codigo));
        produto.codigo[strcspn(produto.codigo, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.nome, buffer, sizeof(produto.nome));
        produto.nome[strcspn(produto.nome, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.quantidade = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.preco = atof(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.estado, buffer, sizeof(produto.estado));
        produto.estado[strcspn(produto.estado, "\n")] = '\0'; 

        if (strcmp(produto.estado, estado_procurado) == 0 && produto.quantidade < menor_quantidade) {
            menor_quantidade = produto.quantidade;
            menor_produto = produto;
            encontrado = 1;
        }
    }

    if (encontrado) {
        printf("%s\n", menor_produto.codigo);
        printf("%s\n", menor_produto.nome);
        printf("%d\n", menor_produto.quantidade);
        printf("%.2f\n", menor_produto.preco);
        printf("%s\n", menor_produto.estado);
    } else {
        printf("Nenhum produto encontrado para o estado %s.\n", estado_procurado);
    }

    fclose(in);
}

void calcular_quantidade_total(const char *input_file) {
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    ignorar_primeira_linha(in);

    Produto produto;
    int total_quantidade = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        strncpy(produto.codigo, buffer, sizeof(produto.codigo));
        produto.codigo[strcspn(produto.codigo, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.nome, buffer, sizeof(produto.nome));
        produto.nome[strcspn(produto.nome, "\n")] = '\0'; 

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.quantidade = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        produto.preco = atof(buffer);

        if (fgets(buffer, sizeof(buffer), in) == NULL) break;
        strncpy(produto.estado, buffer, sizeof(produto.estado));
        produto.estado[strcspn(produto.estado, "\n")] = '\0'; 

        total_quantidade += produto.quantidade;
    }

    printf("%d\n", total_quantidade);

    fclose(in);
}

void abrir_arquivo(const char *input_file, int funcao, const char *param) {
    switch (funcao) {
        case 1:
            copiar_arquivo_ignorando_primeira_linha(input_file, param);
            break;
        case 2:
            pesquisar_produto(input_file, param);
            break;
        case 3:
            listar_produto_menor_quantidade(input_file);
            break;
        case 4:
            listar_produtos_por_estado(input_file, param);
            break;
        case 5:
            encontrar_produto_menor_quantidade_estado(input_file, param);
            break;
        case 6:
            calcular_quantidade_total(input_file);
            break;
        default:
            printf("Opção inválida\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Uso: %s <nome_do_arquivo_entrada> <funcao> [parametro]\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    int funcao = atoi(argv[2]);
    const char *param = argc == 4 ? argv[3] : NULL;

    abrir_arquivo(input_file, funcao, param);

    return 0;
}