#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movimentacao.h"
#include "utils.h"
// Não precisa de validadores.h pois não há input direto do usuário

#define MOVIMENTACOES_FILE DATA_DIR PATH_SEPARATOR "movimentacoes.dat"

void registrar_movimentacao(int id_produto, const char *tipo_mov, int quantidade)
{
    Movimentacao nova;
    FILE *arq_movimentacoes;

    // Os dados (id_produto, tipo_mov, quantidade) são assumidos como válidos,
    // pois foram validados no módulo estoque antes de chamar esta função.
    nova.id_produto = id_produto;
    // O tipo já está validado para ser "Entrada" ou "Saida" no estoque.c
    strncpy(nova.tipo, tipo_mov, sizeof(nova.tipo) - 1);
    nova.tipo[sizeof(nova.tipo) - 1] = '\0';
    nova.quantidade = quantidade;

    // data e hora atual para registrar a movimentação
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(nova.data, sizeof(nova.data), "%d/%m/%Y", &tm);

    arq_movimentacoes = fopen(MOVIMENTACOES_FILE, "ab");
    if (arq_movimentacoes == NULL)
    {
        printf("ERRO CRITICO: Nao foi possivel abrir o arquivo de movimentacoes!\n");
        return;
    }

    fwrite(&nova, sizeof(Movimentacao), 1, arq_movimentacoes);
    fclose(arq_movimentacoes);
}

// função para ler e exibir o histórico, chamada nos relatórios.
void exibir_historico_movimentacoes(void)
{
    Movimentacao mov_lida;
    FILE *arq_movimentacoes;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║       Historico de Movimentacoes       ║\n");
    printf("╚════════════════════════════════════════╝\n");

    arq_movimentacoes = fopen(MOVIMENTACOES_FILE, "rb");
    if (arq_movimentacoes == NULL)
    {
        printf("Nenhuma movimentacao de estoque registrada.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔════════════╦════════════╦════════════╦═════════════════╗\n");
    printf("║ ID Produto ║ Tipo       ║ Quantidade ║ Data            ║\n");
    printf("╠════════════╬════════════╬════════════╬═════════════════╣\n");

    while (fread(&mov_lida, sizeof(Movimentacao), 1, arq_movimentacoes))
    {
        printf("║ %-10d ║ %-10s ║ %-10d ║ %-15s ║\n",
               mov_lida.id_produto, mov_lida.tipo,
               mov_lida.quantidade, mov_lida.data);
    }
    printf("╚════════════╩════════════╩════════════╩═════════════════╝\n");

    fclose(arq_movimentacoes);
    pressioneEnterParaContinuar();
}

// ===============================================
// HISTÓRICO ORDENADO DE MOVIMENTAÇÕES
// ===============================================

// Funções de comparação para movimentações
static int comparar_movimentacoes_quantidade_desc(const void *a, const void *b)
{
    Movimentacao *movA = (Movimentacao *)a;
    Movimentacao *movB = (Movimentacao *)b;
    return movB->quantidade - movA->quantidade;
}

static int comparar_movimentacoes_quantidade_asc(const void *a, const void *b)
{
    Movimentacao *movA = (Movimentacao *)a;
    Movimentacao *movB = (Movimentacao *)b;
    return movA->quantidade - movB->quantidade;
}

void exibir_historico_movimentacoes_ordenado(void)
{
    FILE *arq_movimentacoes;
    Movimentacao *movimentacoes = NULL;
    int contador = 0;
    int capacidade = 10;
    int opcao;
    char bufferOpcao[5];

    limparTela();
    printf("╔═════════════════════════════════════════╗\n");
    printf("║   Historico Ordenado de Movimentacoes   ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║ 1. Ordenar por Quantidade (maior→menor) ║\n");
    printf("║ 2. Ordenar por Quantidade (menor→maior) ║\n");
    printf("║ 0. Voltar                               ║\n");
    printf("╚═════════════════════════════════════════╝\n");

    do
    {
        printf(">>> Escolha a opcao: ");
        lerString(bufferOpcao, 5);
        char *endptr;
        opcao = strtol(bufferOpcao, &endptr, 10);
        if (endptr == bufferOpcao || *endptr != '\0')
        {
            opcao = -1;
        }
    } while (opcao < 0 || opcao > 2);

    if (opcao == 0)
    {
        return;
    }

    // Alocar memória inicial
    movimentacoes = (Movimentacao *)malloc(capacidade * sizeof(Movimentacao));
    if (movimentacoes == NULL)
    {
        printf("\nErro ao alocar memoria!\n");
        pressioneEnterParaContinuar();
        return;
    }

    // Carregar movimentações do arquivo
    arq_movimentacoes = fopen(MOVIMENTACOES_FILE, "rb");
    if (arq_movimentacoes == NULL)
    {
        printf("\nNenhuma movimentacao registrada.\n");
        free(movimentacoes);
        pressioneEnterParaContinuar();
        return;
    }

    Movimentacao mov_lida;
    while (fread(&mov_lida, sizeof(Movimentacao), 1, arq_movimentacoes) == 1)
    {
        // Expandir array se necessário
        if (contador == capacidade)
        {
            capacidade *= 2;
            movimentacoes = (Movimentacao *)realloc(movimentacoes, capacidade * sizeof(Movimentacao));
            if (movimentacoes == NULL)
            {
                printf("\nErro ao realocar memoria!\n");
                fclose(arq_movimentacoes);
                pressioneEnterParaContinuar();
                return;
            }
        }
        movimentacoes[contador++] = mov_lida;
    }
    fclose(arq_movimentacoes);

    if (contador == 0)
    {
        printf("\nNenhuma movimentacao para exibir.\n");
        free(movimentacoes);
        pressioneEnterParaContinuar();
        return;
    }

    // Ordenar conforme escolha
    limparTela();
    if (opcao == 1)
    {
        qsort(movimentacoes, contador, sizeof(Movimentacao), comparar_movimentacoes_quantidade_desc);
        printf("╔═════════════════════════════════════════╗\n");
        printf("║  Movimentacoes por Qtd (Maior → Menor)  ║\n");
        printf("╚═════════════════════════════════════════╝\n");
    }
    else
    {
        qsort(movimentacoes, contador, sizeof(Movimentacao), comparar_movimentacoes_quantidade_asc);
        printf("╔═════════════════════════════════════════╗\n");
        printf("║  Movimentacoes por Qtd (Menor → Maior)  ║\n");
        printf("╚═════════════════════════════════════════╝\n");
    }

    // Exibir movimentações ordenadas
    printf("╔════════════╦════════════╦════════════╦═════════════════╗\n");
    printf("║ ID Produto ║ Tipo       ║ Quantidade ║ Data            ║\n");
    printf("╠════════════╬════════════╬════════════╬═════════════════╣\n");

    for (int i = 0; i < contador; i++)
    {
        printf("║ %-10d ║ %-10s ║ %-10d ║ %-15s ║\n",
               movimentacoes[i].id_produto,
               movimentacoes[i].tipo,
               movimentacoes[i].quantidade,
               movimentacoes[i].data);
    }
    printf("╚════════════╩════════════╩════════════╩═════════════════╝\n");

    free(movimentacoes);
    pressioneEnterParaContinuar();
}
